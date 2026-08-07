#!/usr/bin/env python3
"""Validate the source-backed retail profile-name enumeration contract."""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


CALLS = [
    "FableProfileNamesCall_00412450",
    "FableProfileNamesCall_0041a540",
    "FableProfileNamesCall_0099bf30",
    "FableProfileNamesCall_0099b510",
    "FableProfileNamesCall_00999760",
    "FableProfileNamesCall_0099b6b0",
    "FableProfileNamesCall_0099ebf0",
    "FableProfileNamesCall_0099b2c0",
    "FableProfileNamesCall_0099eae0",
    "FableProfileNamesCall_00999a30",
    "FableProfileNamesCall_0099b940",
    "FableProfileNamesCall_0099aed0",
    "FableProfileNamesCall_00997620",
    "FableProfileNamesCall_0099b720",
    "FableProfileNamesCall_00999850",
    "FableProfileNamesCall_00bfe9f9",
]


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    root = args.root.resolve()
    source_path = (
        root
        / "rebuild"
        / "src"
        / "compiled"
        / "00"
        / "59"
        / "CFrontEndManager_GetProfileNames_005955ab.cpp"
    )
    contract_path = (
        root
        / "work"
        / "frontend_re"
        / "CFrontEndManager_GetProfileNames_005955ab_contract.md"
    )
    errors: list[str] = []
    try:
        source = source_path.read_text(encoding="utf-8")
        contract = contract_path.read_text(encoding="utf-8")
    except OSError as error:
        print(f"FRONTEND_PROFILE_NAMES_CONTRACT FAIL {error}")
        return 1

    body_match = re.search(
        r"CFrontEndManager_GetProfileNames_005955ab\(void\*\)\n\{(.*?)\n\}",
        source,
        re.DOTALL,
    )
    if body_match is None:
        errors.append("retail candidate body not found")
        body = ""
    else:
        body = body_match.group(1)

    positions = []
    for call in CALLS:
        position = body.find(call)
        if position < 0:
            errors.append(f"missing retail helper seam: {call}")
        else:
            positions.append((position, call))
    if positions != sorted(positions):
        errors.append("first helper-seam order is not monotonic")

    for literal in (
        "FableProfileNamesDefaultPattern_005955ab",
        "FableProfileNamesWildcard_005955ab",
        "FableProfileNamesEmpty_005955ab",
    ):
        if literal not in source:
            errors.append(f"missing retail literal seam: {literal}")
    for evidence in (
        "558",
        "0x005955AB",
        "0x00997620",
        "0x00999760",
        "0x00999850",
        "C4234",
    ):
        if evidence not in contract:
            errors.append(f"contract evidence missing: {evidence}")

    if errors:
        for error in errors:
            print(f"FRONTEND_PROFILE_NAMES_CONTRACT FAIL {error}")
        return 1
    print(
        "FRONTEND_PROFILE_NAMES_CONTRACT PASS "
        f"helpers={len(CALLS)} literals=3 source={source_path.name}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
