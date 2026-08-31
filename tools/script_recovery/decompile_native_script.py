#!/usr/bin/env python3
"""Orchestrate anchor-based allocator discovery and evidence-gated Ghidra export."""

from __future__ import annotations

import argparse
import json
import subprocess
from pathlib import Path
from typing import Any


def parse_discovery(output: str) -> list[dict[str, Any]]:
    inside = False
    rows = []
    for raw_line in output.splitlines():
        line = raw_line.strip()
        if "DISCOVERY_BEGIN" in line:
            inside = True
            continue
        if "DISCOVERY_END" in line:
            inside = False
            continue
        if inside:
            start, end = line.find("{"), line.rfind("}")
            if start >= 0 and end > start:
                rows.append(json.loads(line[start:end + 1]))
    return rows


def run_headless(headless: Path, project_dir: Path, project_name: str, program: str,
                 script_dir: Path, post_script: str, script_args: list[str]) -> str:
    command = [
        str(headless), str(project_dir), project_name, "-process", program, "-noanalysis",
        "-scriptPath", str(script_dir), "-postScript", post_script, *script_args,
    ]
    result = subprocess.run(command, check=False, text=True, capture_output=True)
    combined = result.stdout + result.stderr
    if result.returncode != 0 or "SCRIPT ERROR" in combined:
        raise RuntimeError(f"Ghidra {post_script} failed (exit {result.returncode})\n{combined}")
    return combined


def decompile(headless: Path, project_dir: Path, project_name: str, program: str,
              script_dir: Path, script_name: str, anchor: str, output: Path) -> dict[str, Any]:
    discovery_log = run_headless(
        headless, project_dir, project_name, program, script_dir,
        "DiscoverScriptAllocatorFromAnchor.java", [anchor],
    )
    candidates = parse_discovery(discovery_log)
    if len(candidates) != 1:
        raise RuntimeError(f"expected exactly one allocator candidate for {script_name}/{anchor}, got {len(candidates)}")
    allocator = candidates[0]["allocatorCandidate"]
    run_headless(
        headless, project_dir, project_name, program, script_dir,
        "ExportNativeScriptCluster.java", [script_name, allocator, str(output), anchor],
    )
    payload = json.loads(output.read_text(encoding="utf-8-sig"))
    if payload["allocatorAddress"].lower() != allocator.lower() or anchor not in payload["evidenceAnchors"]:
        raise RuntimeError("exported cluster failed allocator/anchor postcondition")
    return payload


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--headless", type=Path, required=True)
    parser.add_argument("--project-dir", type=Path, required=True)
    parser.add_argument("--project-name", required=True)
    parser.add_argument("--program", default="Fable.exe")
    parser.add_argument("--script-dir", type=Path, required=True)
    parser.add_argument("--script-name", required=True)
    parser.add_argument("--anchor", required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    payload = decompile(args.headless, args.project_dir, args.project_name, args.program,
                        args.script_dir, args.script_name, args.anchor, args.output)
    print(json.dumps({"script": payload["script"], "allocator": payload["allocatorAddress"],
                      "output": str(args.output.resolve())}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
