#!/usr/bin/env python3
"""Turn corpus IR and validation findings into a ForgeFSE compatibility backlog."""

from __future__ import annotations

import argparse
import json
from collections import defaultdict
from pathlib import Path
from typing import Any


def analyze(corpus_root: Path, json_output: Path, markdown_output: Path) -> dict[str, Any]:
    index = json.loads((corpus_root / "corpus_index.json").read_text(encoding="utf-8-sig"))
    validation = json.loads((corpus_root / "validation.json").read_text(encoding="utf-8-sig"))
    calls: dict[tuple[str, str], dict[str, Any]] = {}
    trace_templates = []
    for package in index["packages"]:
        for file_row in package["files"]:
            if "ir" not in file_row:
                continue
            ir = json.loads((corpus_root / file_row["ir"]).read_text(encoding="utf-8-sig"))
            functions = []
            for function in ir["functions"]:
                events = []
                for call in function["calls"]:
                    scope = "Entity" if call["receiver"] == "me" else "Quest"
                    key = (scope, call["name"])
                    row = calls.setdefault(key, {"scope": scope, "name": call["name"], "count": 0, "consumers": set()})
                    row["count"] += 1
                    row["consumers"].add(f"{package['name']}:{file_row['path']}")
                    events.append({"event": "api-call", "scope": scope, "name": call["name"], "sourceLine": call["line"],
                                   "arguments": "unresolved", "result": "fixture-supplied"})
                functions.append({"name": function["name"], "events": events})
            trace_templates.append({"package": package["name"], "script": file_row["path"], "kind": ir["kind"],
                                    "evidenceLevel": "reconstructed-source", "functions": functions})

    missing: dict[str, dict[str, Any]] = {}
    for finding in validation:
        if finding["code"] != "unknown-api":
            continue
        message = finding["message"]
        name = message.split("'")[1]
        row = missing.setdefault(name, {"name": name, "occurrences": 0, "consumers": set()})
        row["occurrences"] += 1
        row["consumers"].add(f"{finding['package']}:{finding['path']}")

    call_rows = []
    for row in calls.values():
        row["consumers"] = sorted(row["consumers"])
        call_rows.append(row)
    missing_rows = []
    for row in missing.values():
        row["consumers"] = sorted(row["consumers"])
        row["priority"] = row["occurrences"] * len(row["consumers"])
        missing_rows.append(row)
    missing_rows.sort(key=lambda row: (-row["priority"], row["name"]))
    call_rows.sort(key=lambda row: (-row["count"], row["scope"], row["name"]))

    payload = {
        "schema": "forgefse-script-compatibility/0.1",
        "corpus": str(corpus_root.resolve()),
        "summary": {"uniqueCalls": len(call_rows), "totalCalls": sum(row["count"] for row in call_rows),
                    "missingApis": len(missing_rows), "traceTemplates": len(trace_templates)},
        "missingApis": missing_rows,
        "calls": call_rows,
        "traceTemplates": trace_templates,
    }
    json_output.parent.mkdir(parents=True, exist_ok=True)
    json_output.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")

    lines = [
        "# ForgeFSE seed-script compatibility report", "",
        f"- Scripts with trace templates: **{len(trace_templates)}**",
        f"- API call sites: **{payload['summary']['totalCalls']}** across **{len(call_rows)}** scope/name pairs",
        f"- Missing API names: **{len(missing_rows)}**", "", "## Missing capabilities", "",
        "| Priority | API | Occurrences | Consumers |", "|---:|---|---:|---|",
    ]
    for row in missing_rows:
        consumers = "<br>".join(row["consumers"])
        lines.append(f"| {row['priority']} | `{row['name']}` | {row['occurrences']} | {consumers} |")
    lines.extend([
        "", "## Interpretation", "",
        "A missing capability means the reconstructed scripts call a name that is absent from the current",
        "ForgeFSE/tutorial API manifest. It is a conversion blocker, not proof that the proposed signature or",
        "behavior is correct. Each capability still needs native address, ABI, wrapper, and runtime validation.", "",
        "Trace templates are ordered lexical call skeletons. Arguments and branch outcomes remain unresolved",
        "until the native decompiler or a runtime fixture supplies them.", "",
    ])
    markdown_output.parent.mkdir(parents=True, exist_ok=True)
    markdown_output.write_text("\n".join(lines), encoding="utf-8")
    return payload["summary"]


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--corpus", type=Path, required=True)
    parser.add_argument("--json", type=Path, required=True)
    parser.add_argument("--markdown", type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(analyze(args.corpus.resolve(), args.json.resolve(), args.markdown.resolve()), sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

