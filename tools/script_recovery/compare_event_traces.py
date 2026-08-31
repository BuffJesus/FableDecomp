#!/usr/bin/env python3
"""Compare ForgeFSE Lua and native/shadow semantic event traces."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Any


def normalize(events: list[dict[str, Any]], ignore: set[str]) -> list[dict[str, Any]]:
    result = []
    for event in events:
        if event.get("name") in ignore:
            continue
        result.append({key: value for key, value in event.items() if key not in {"timestamp", "sourceLine"}})
    return result


def compare(expected: list[dict[str, Any]], actual: list[dict[str, Any]], mode: str = "exact",
            ignore: set[str] | None = None) -> dict[str, Any]:
    ignore = ignore or set()
    left, right = normalize(expected, ignore), normalize(actual, ignore)
    if mode == "exact":
        matched = left == right
        first_difference = next((index for index, pair in enumerate(zip(left, right)) if pair[0] != pair[1]), None)
        if first_difference is None and len(left) != len(right):
            first_difference = min(len(left), len(right))
    elif mode == "subsequence":
        cursor = 0
        for event in right:
            if cursor < len(left) and left[cursor] == event:
                cursor += 1
        matched = cursor == len(left)
        first_difference = None if matched else cursor
    else:
        raise ValueError(f"unsupported comparison mode {mode!r}")
    return {
        "matched": matched,
        "mode": mode,
        "expectedEvents": len(left),
        "actualEvents": len(right),
        "firstDifference": first_difference,
        "expectedAtDifference": left[first_difference] if first_difference is not None and first_difference < len(left) else None,
        "actualAtDifference": right[first_difference] if first_difference is not None and first_difference < len(right) else None,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("expected", type=Path)
    parser.add_argument("actual", type=Path)
    parser.add_argument("--mode", choices=("exact", "subsequence"), default="exact")
    parser.add_argument("--ignore", action="append", default=[])
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    expected = json.loads(args.expected.read_text(encoding="utf-8-sig"))["events"]
    actual = json.loads(args.actual.read_text(encoding="utf-8-sig"))["events"]
    result = compare(expected, actual, args.mode, set(args.ignore))
    text = json.dumps(result, indent=2, sort_keys=True) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text, encoding="utf-8")
    print(text, end="")
    return 0 if result["matched"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
