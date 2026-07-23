#!/usr/bin/env python3
"""Validate generated RE SDK artifacts before downstream tools consume them."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
from pathlib import Path


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--skip-mirrors", action="store_true")
    args = parser.parse_args()
    root = args.root.resolve()
    manifest_path = root / "rebuild" / "manifest" / "functions.tsv"
    overlay_path = root / "rebuild" / "sdk" / "fse_native_overlay.json"
    header_path = root / "ghidra_out" / "fable_engine.h"
    with manifest_path.open("r", encoding="utf-8-sig", newline="") as stream:
        functions = list(csv.DictReader(stream, delimiter="\t"))
    by_address = {f"0x{int(row['address'], 16):08X}": row for row in functions}
    overlay = json.loads(overlay_path.read_text(encoding="utf-8-sig"))
    errors: list[str] = []
    if overlay.get("schemaVersion") != "1.1.0":
        errors.append(f"expected overlay schema 1.1.0, found {overlay.get('schemaVersion')!r}")
    policy = overlay.get("policy", {})
    if policy.get("recommendationIsNotHookApproval") is not True:
        errors.append("overlay policy must separate recommendations from hook approval")
    if policy.get("engineVerificationIsNotBindingVerification") is not True:
        errors.append("overlay policy must separate engine verification from binding verification")
    fse_functions = overlay.get("functions", [])
    if len(fse_functions) != 931:
        errors.append(f"expected 931 FSE functions, found {len(fse_functions)}")
    names = [(item.get("scope"), item.get("name")) for item in fse_functions]
    if len(names) != len(set(names)):
        errors.append("duplicate scope/name rows in FSE overlay")
    candidate_count = 0
    recommended_count = 0
    hook_approved_count = 0
    for item in fse_functions:
        candidates = item.get("candidates", [])
        addresses = {candidate.get("address") for candidate in candidates}
        candidate_count += len(candidates)
        recommended = item.get("recommendedAddress")
        if item.get("hookApproved") is not False:
            errors.append(f"{item['scope']}::{item['name']}: generated overlays must not approve hooks")
            hook_approved_count += 1
        if recommended:
            recommended_count += 1
            if recommended not in addresses:
                errors.append(f"{item['scope']}::{item['name']}: recommendation not in candidates")
        for candidate in candidates:
            address = candidate.get("address")
            row = by_address.get(address)
            if row is None:
                errors.append(f"{item['name']}: unknown candidate address {address}")
                continue
            leaf = row["name"].rsplit("::", 1)[-1]
            if leaf.casefold() != item["name"].casefold():
                errors.append(f"{item['name']}: leaf mismatch at {address}: {leaf}")
            if candidate.get("qualifiedName") != row["name"]:
                errors.append(f"{item['name']}: qualified-name drift at {address}")
            if candidate.get("prototypeComplete") != (row["prototype_complete"] == "1"):
                errors.append(f"{item['name']}: prototype state drift at {address}")
            expected_rva = f"0x{int(row['address'], 16) - 0x00400000:08X}"
            if candidate.get("rva") != expected_rva:
                errors.append(f"{item['name']}: RVA mismatch at {address}")
        if recommended:
            selected = next(candidate for candidate in candidates if candidate.get("address") == recommended)
            if not selected.get("prototypeComplete"):
                errors.append(f"{item['name']}: incomplete prototype was recommended")
            if selected.get("ownerRelevance") not in ("strong", "moderate"):
                errors.append(f"{item['name']}: owner-irrelevant candidate was recommended")
            if item.get("recommendationConfidence") != "owner-aligned-candidate":
                errors.append(f"{item['name']}: recommendation confidence missing or invalid")
    verified = overlay.get("verifiedEngineFunctions", [])
    for candidate in verified:
        if not candidate.get("retailParity") and not candidate.get("liftGrade"):
            errors.append(f"verified engine function lacks verification evidence: {candidate.get('address')}")
        if candidate.get("engineImplementationVerified") is not True:
            errors.append(f"verified engine function flag is false: {candidate.get('address')}")
    summary = overlay.get("summary", {})
    expected_summary = {
        "fseFunctions": len(fse_functions),
        "ownerAlignedRecommendations": recommended_count,
        "verifiedEngineFunctions": len(verified),
        "hookApprovedBindings": hook_approved_count,
    }
    for key, expected in expected_summary.items():
        if summary.get(key) != expected:
            errors.append(f"summary {key}={summary.get(key)!r}, expected {expected}")
    header_text = header_path.read_text(encoding="utf-8", errors="replace")
    header_functions = len(re.findall(r"^\s+constexpr std::uintptr_t ", header_text, re.MULTILINE))
    if header_functions != len(functions):
        errors.append(f"engine header has {header_functions} constants, expected {len(functions)}")
    if not args.skip_mirrors:
        for mirror in (
            Path(r"D:\Code\FableForge\generated\fable_engine.h"),
            Path(r"D:\Code\FableForge\docs\re_reference\fse_native_overlay.json"),
            Path(r"D:\Code\ForgeFSE\docs\fse_native_overlay.json"),
        ):
            canonical = header_path if mirror.name == "fable_engine.h" else overlay_path
            if not mirror.exists():
                errors.append(f"missing mirror: {mirror}")
            elif sha256(mirror) != sha256(canonical):
                errors.append(f"mirror drift: {mirror}")
    report = {
        "status": "PASS" if not errors else "FAIL",
        "engineFunctions": len(functions),
        "headerFunctions": header_functions,
        "fseFunctions": len(fse_functions),
        "nativeCandidates": candidate_count,
        "recommendedCandidates": recommended_count,
        "verifiedEngineFunctions": len(verified),
        "hookApprovedBindings": hook_approved_count,
        "errors": errors[:100],
    }
    print(json.dumps(report, indent=2))
    return 0 if not errors else 1


if __name__ == "__main__":
    raise SystemExit(main())
