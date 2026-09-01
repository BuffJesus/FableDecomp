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


def analyze(source_path: Path) -> dict[str, Any]:
    source = json.loads(source_path.read_text(encoding="utf-8-sig"))
    rows = []
    for helper in source["helpers"]:
        text = helper.get("decompile") or ""
        direct = helper.get("directCalls", [])
        parsed = correlate_direct_call_targets(calls(text), direct) if text else []
        controls = Counter(match.group(1) for match in re.finditer(
            r"\b(if|while|for|switch|goto)\b", text))
        writes = [{"fieldOffset": match.group(1).lower(),
                   "valueExpression": match.group(2).strip(), "offset": match.start()}
                  for match in STATE_WRITE_RE.finditer(text)]
        status = helper["status"]
        if status != "decompiled":
            stage = "repair-helper-decompile"
        elif not direct and not indirect_calls(text):
            stage = "inspect-leaf-helper"
        else:
            stage = "inspect-helper-dependencies"
        rows.append({
            "targetAddress": helper["targetAddress"], "currentName": helper["currentName"],
            "helperNames": [name.strip() for name in helper["helperNames"].split("|")],
            "category": helper["category"], "consumerCalls": helper["calls"],
            "consumerScripts": helper["scripts"],
            "consumerRoles": [role for role in helper["roles"].split(",") if role],
            "status": status, "error": helper.get("error"), "stage": stage,
            "decompileSha256": hashlib.sha256(text.encode("utf-8")).hexdigest() if text else None,
            "decompileLines": len(text.splitlines()) if text else 0,
            "calls": parsed, "directCallTargets": direct, "indirectCalls": indirect_calls(text),
            "strings": strings(text), "stateWrites": writes,
            "controlFlow": dict(sorted(controls.items())),
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
            "stages": dict(sorted(stages.items())),
            "categories": dict(sorted(categories.items())),
        },
        "helpers": rows,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    result = analyze(args.source)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["summary"], sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
