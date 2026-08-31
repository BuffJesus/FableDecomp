#!/usr/bin/env python3
"""Verify ForgeFSE retail-shadow startup and per-script completion from its log."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


def verify(log_path: Path, manifest_path: Path) -> dict[str, object]:
    log = log_path.read_text(encoding="utf-8", errors="replace")
    manifest = json.loads(manifest_path.read_text(encoding="utf-8-sig"))
    expected = int(manifest["expectedScripts"])
    completed = set(re.findall(r"\[RetailShadow\] completed mutation-free preflight for (.+)$", log, re.MULTILINE))
    summary = re.findall(r"Retail shadow preflight complete: passed=(\d+) failed=(\d+)", log)
    passed, failed = (map(int, summary[-1])) if summary else (0, -1)
    result = {
        "schema": "forgefse-retail-shadow-smoke-result/0.1",
        "dllAttached": "--- Fable Custom Quest DLL Attached ---" in log,
        "luaInitialized": "--- Lua systems initialized successfully. ---" in log,
        "expectedScripts": expected,
        "completedScripts": len(completed),
        "passed": passed,
        "failed": failed,
        "errors": len(re.findall(r"\[RetailShadow\].*(?:failed|exception|missing candidate)", log)),
    }
    result["complete"] = all((result["dllAttached"], result["luaInitialized"],
                              len(completed) == expected, passed == expected,
                              failed == 0, result["errors"] == 0))
    return result


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--log", type=Path, required=True)
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = verify(args.log.resolve(), args.manifest.resolve())
    if args.output:
        args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result, sort_keys=True))
    return 0 if result["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
