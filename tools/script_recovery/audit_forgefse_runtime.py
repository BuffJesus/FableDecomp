#!/usr/bin/env python3
"""Audit recovered script calls against bindings actually registered by ForgeFSE."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
from typing import Any


BINDING_RE = re.compile(r'\b(questState_type|cscriptThing_type)\s*\[\s*"([A-Za-z_][A-Za-z0-9_]*)"\s*\]')


def extract_bindings(lua_manager: Path) -> set[tuple[str, str]]:
    text = lua_manager.read_text(encoding="utf-8-sig", errors="replace")
    result = set()
    for owner, name in BINDING_RE.findall(text):
        result.add(("Quest" if owner == "questState_type" else "Entity", name))
    return result


def audit(compatibility_path: Path, lua_manager: Path, json_output: Path, markdown_output: Path) -> dict[str, int]:
    compatibility = json.loads(compatibility_path.read_text(encoding="utf-8-sig"))
    bindings = extract_bindings(lua_manager)
    rows = []
    for call in compatibility["calls"]:
        key = (call["scope"], call["name"])
        other = ("Entity" if call["scope"] == "Quest" else "Quest", call["name"])
        status = "callable" if key in bindings else "wrong-scope" if other in bindings else "missing"
        rows.append({**call, "status": status})
    rows.sort(key=lambda row: ({"missing": 0, "wrong-scope": 1, "callable": 2}[row["status"]], -row["count"], row["name"]))
    summary = {
        "registeredQuestBindings": sum(scope == "Quest" for scope, _ in bindings),
        "registeredEntityBindings": sum(scope == "Entity" for scope, _ in bindings),
        "requiredCalls": len(rows),
        "callable": sum(row["status"] == "callable" for row in rows),
        "missing": sum(row["status"] == "missing" for row in rows),
        "wrongScope": sum(row["status"] == "wrong-scope" for row in rows),
    }
    payload: dict[str, Any] = {
        "schema": "forgefse-runtime-script-audit/0.1",
        "source": str(lua_manager.resolve()),
        "summary": summary,
        "calls": rows,
    }
    json_output.parent.mkdir(parents=True, exist_ok=True)
    json_output.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
    lines = ["# ForgeFSE runtime binding audit", "", *(f"- {key}: **{value}**" for key, value in summary.items()),
             "", "## Conversion blockers", "", "| Status | Scope | API | Calls | Consumers |", "|---|---|---|---:|---|"]
    for row in rows:
        if row["status"] == "callable":
            continue
        lines.append(f"| {row['status']} | {row['scope']} | `{row['name']}` | {row['count']} | {'<br>'.join(row['consumers'])} |")
    lines.extend(["", "This report reflects names registered in `LuaManager.cpp`; it does not prove native ABI or runtime behavior.", ""])
    markdown_output.write_text("\n".join(lines), encoding="utf-8")
    return summary


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--compatibility", type=Path, required=True)
    parser.add_argument("--lua-manager", type=Path, required=True)
    parser.add_argument("--json", type=Path, required=True)
    parser.add_argument("--markdown", type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(audit(args.compatibility.resolve(), args.lua_manager.resolve(),
                           args.json.resolve(), args.markdown.resolve()), sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

