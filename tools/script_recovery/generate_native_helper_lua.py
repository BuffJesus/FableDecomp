#!/usr/bin/env python3
"""Emit standalone Lua only for completely recognized native helper semantics."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
from typing import Any


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest().upper()


def emit_switch(helper: dict[str, Any], pattern: dict[str, Any]) -> str:
    lines = [
        f"-- Retail helper {helper['targetAddress']} ({helper['currentName']})",
        "-- Generated only from a complete native constant-return switch.",
        "local cases = {",
    ]
    for case in pattern["cases"]:
        lines.append(f"    [{case['input']}] = {case['return']},")
    lines.extend(("}", "", "return function(value)",
                  f"    return cases[value] or {pattern['defaultReturn']}", "end", ""))
    return "\n".join(lines)


def generate(helper_ir_path: Path, output_dir: Path) -> dict[str, Any]:
    source_bytes = helper_ir_path.read_bytes()
    document = json.loads(source_bytes.decode("utf-8-sig"))
    output_dir.mkdir(parents=True, exist_ok=True)
    entries = []
    for helper in document["helpers"]:
        if not helper.get("luaEmissionReady"):
            continue
        patterns = [row for row in helper["semanticPatterns"]
                    if row["kind"] == "constant-return-switch" and row["complete"]]
        if len(patterns) != 1:
            raise ValueError(f"expected one complete switch for {helper['targetAddress']}")
        pattern = patterns[0]
        text = emit_switch(helper, pattern)
        filename = helper["targetAddress"].removeprefix("0x") + ".lua"
        destination = output_dir / filename
        destination.write_text(text, encoding="utf-8", newline="\n")
        entries.append({
            "targetAddress": helper["targetAddress"], "currentName": helper["currentName"],
            "sourceDecompileSha256": helper["decompileSha256"],
            "luaFile": filename, "luaSha256": sha256(text.encode("utf-8")),
            "semanticPattern": pattern,
            "deploymentEligible": False,
            "deploymentBlocker": "standalone helper only; parent script state/control flow unresolved",
        })
    manifest = {
        "schema": "forgefse-native-helper-lua/0.1",
        "source": str(helper_ir_path.resolve()),
        "sourceSha256": sha256(source_bytes),
        "summary": {"emitted": len(entries), "deploymentEligible": 0},
        "entries": entries,
    }
    (output_dir / "manifest.json").write_text(
        json.dumps(manifest, indent=2) + "\n", encoding="utf-8", newline="\n")
    return manifest


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--helper-ir", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path, required=True)
    args = parser.parse_args()
    result = generate(args.helper_ir, args.output_dir)
    print(json.dumps(result["summary"], sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
