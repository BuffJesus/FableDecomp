#!/usr/bin/env python3
"""Reject reconstructed entity speech whose termination result is silently discarded."""
from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


CALL_RE = re.compile(r"\b(?:me:Speak|speak_if_alive|speak_to_hero)\s*\(")


def audit(entities_dir: Path) -> dict:
    violations = []
    checked = 0
    for path in sorted(entities_dir.glob("*.lua")):
        for number, raw in enumerate(path.read_text(encoding="utf-8-sig").splitlines(), 1):
            code = raw.split("--", 1)[0].strip()
            if not code or "local function speak_" in code or not CALL_RE.search(code):
                continue
            checked += 1
            # Accepted forms either return the result, branch on it, or assign it for a later branch.
            if not ("return " in code or code.startswith(("if ", "or not "))
                    or re.search(r"\b\w+\s*=\s*", code)):
                violations.append({"file": path.name, "line": number, "code": code})
    return {"schema": "reconstructed-speech-propagation-audit/0.1", "ok": not violations,
            "callsChecked": checked, "violations": violations}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--entities-dir", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.entities_dir)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
