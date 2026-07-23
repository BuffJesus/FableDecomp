#!/usr/bin/env python3
"""Join the FSE Lua API to exact-name retail engine candidates with evidence."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
from datetime import datetime
from pathlib import Path


IMAGE_BASE = 0x00400000


def read_functions(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def owner_relevance(module: str, scope: str) -> tuple[str, int, str | None]:
    """Classify whether an exact leaf-name match lives in a plausible FSE owner."""
    if scope == "Entity":
        if module in ("CScriptThing", "CGameScriptThing"):
            return "strong", 60, "entity-owner"
        if "ScriptThing" in module:
            return "strong", 45, "entity-family-owner"
        if module == "CGameScriptInterface":
            return "moderate", 25, "game-script-interface-owner"
    elif scope == "Quest":
        if module == "CGameScriptInterface":
            return "strong", 60, "quest-interface-owner"
        if "Quest" in module or module in ("CWorld", "CThingManager", "CHero"):
            return "moderate", 35, "quest-runtime-owner"
    return "none", 0, None


def score(row: dict[str, str], scope: str) -> tuple[int, list[str]]:
    value = 20
    reasons = ["exact-leaf-name"]
    module = row["module"]
    if row["prototype_complete"] == "1":
        value += 20
        reasons.append("complete-prototype")
    if row.get("retail_parity") == "MATCH":
        value += 100
        reasons.append("retail-byte-match")
    elif row.get("retail_parity") == "RELOCATION_MATCH":
        value += 95
        reasons.append("retail-relocation-match")
    elif row.get("compiled_status") == "PASS" and row.get("behavior_test") == "PASS":
        value += 55
        reasons.append("compiled-behavior-tested")
    elif row.get("agent_verdict") == "PASS":
        value += 25
        reasons.append("agent-structural-pass")
    if row.get("lift_grade") == "matching":
        value += 90
        reasons.append("verified-matching-lift")
    elif row.get("lift_grade") == "functional":
        value += 70
        reasons.append("verified-functional-lift")
    _, owner_points, owner_reason = owner_relevance(module, scope)
    value += owner_points
    if owner_reason:
        reasons.append(owner_reason)
    return value, reasons


def candidate(row: dict[str, str], scope: str) -> dict[str, object]:
    address = int(row["address"], 16)
    points, reasons = score(row, scope)
    owner_match, _, _ = owner_relevance(row["module"], scope)
    implementation_verified = (
        row.get("retail_parity") in ("MATCH", "RELOCATION_MATCH")
        or row.get("lift_grade") in ("matching", "functional")
    )
    return {
        "address": f"0x{address:08X}",
        "rva": f"0x{address - IMAGE_BASE:08X}",
        "qualifiedName": row["name"],
        "module": row["module"],
        "callingConvention": row["calling_convention"],
        "returnType": row["return_type"],
        "parameterCount": int(row["parameter_count"] or 0),
        "parameterTypesText": row["parameter_types"],
        "prototypeComplete": row["prototype_complete"] == "1",
        "cleanDecompile": row["clean_decompile"],
        "agentVerdict": row.get("agent_verdict", ""),
        "compiledStatus": row.get("compiled_status", ""),
        "behaviorTest": row.get("behavior_test", ""),
        "retailParity": row.get("retail_parity", ""),
        "liftGrade": row.get("lift_grade", ""),
        "ownerRelevance": owner_match,
        "engineImplementationVerified": implementation_verified,
        "score": points,
        "evidence": reasons,
    }


def write_if_changed(path: Path, payload: dict[str, object]) -> bool:
    text = json.dumps(payload, indent=2, ensure_ascii=False) + "\n"
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists() and path.read_text(encoding="utf-8-sig", errors="replace") == text:
        return False
    temp = path.with_suffix(path.suffix + ".tmp")
    temp.write_text(text, encoding="utf-8")
    temp.replace(path)
    return True


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--output", type=Path, action="append")
    parser.add_argument("--no-mirrors", action="store_true")
    args = parser.parse_args()
    root = args.root.resolve()
    engine_path = root / "rebuild" / "manifest" / "functions.tsv"
    fse_path = root / "refs" / "fse_api_manifest.json"
    source_digest = hashlib.sha256(engine_path.read_bytes() + fse_path.read_bytes()).hexdigest()
    canonical_path = root / "rebuild" / "sdk" / "fse_native_overlay.json"
    generated_at = datetime.now().astimezone().isoformat(timespec="seconds")
    if canonical_path.exists():
        try:
            previous = json.loads(canonical_path.read_text(encoding="utf-8-sig"))
            if previous.get("sourceDigest") == source_digest:
                generated_at = previous.get("generatedAt", generated_at)
        except (OSError, json.JSONDecodeError):
            pass
    rows = read_functions(engine_path)
    fse = json.loads(fse_path.read_text(encoding="utf-8-sig"))
    by_leaf: dict[str, list[dict[str, str]]] = {}
    for row in rows:
        leaf = row["name"].rsplit("::", 1)[-1]
        by_leaf.setdefault(leaf.casefold(), []).append(row)

    functions: list[dict[str, object]] = []
    unique = ambiguous = unmatched = recommended = 0
    for function in fse.get("functions", []):
        matches = [candidate(row, function.get("scope", "")) for row in by_leaf.get(function["name"].casefold(), [])]
        matches.sort(key=lambda item: (-int(item["score"]), str(item["address"])))
        if not matches:
            status = "unmatched"
            unmatched += 1
        elif len(matches) == 1:
            status = "unique"
            unique += 1
        else:
            status = "ambiguous"
            ambiguous += 1
        chosen = None
        if (
            matches
            and bool(matches[0]["prototypeComplete"])
            and matches[0]["ownerRelevance"] in ("strong", "moderate")
        ):
            if len(matches) == 1 or int(matches[0]["score"]) >= int(matches[1]["score"]) + 15:
                chosen = matches[0]["address"]
                recommended += 1
        functions.append(
            {
                "name": function["name"],
                "scope": function.get("scope", ""),
                "returnType": function.get("returnType", ""),
                "parameters": function.get("parameters", []),
                "matchStatus": status,
                "recommendedAddress": chosen,
                "recommendationConfidence": "owner-aligned-candidate" if chosen else None,
                "hookApproved": False,
                "candidates": matches,
            }
        )

    verified_engine_functions = []
    for row in rows:
        if row.get("retail_parity") not in ("MATCH", "RELOCATION_MATCH") and row.get("lift_grade") not in ("matching", "functional"):
            continue
        verified_engine_functions.append(candidate(row, ""))
    payload: dict[str, object] = {
        "schemaVersion": "1.1.0",
        "generatedAt": generated_at,
        "sourceDigest": source_digest,
        "target": {"executable": "Fable.exe", "imageBase": "0x00400000", "architecture": "x86"},
        "sources": {
            "fseManifest": "refs/fse_api_manifest.json",
            "engineManifest": "rebuild/manifest/functions.tsv",
        },
        "summary": {
            "fseFunctions": len(functions),
            "uniqueExactNameMatches": unique,
            "ambiguousExactNameMatches": ambiguous,
            "unmatched": unmatched,
            "ownerAlignedRecommendations": recommended,
            "verifiedEngineFunctions": len(verified_engine_functions),
            "hookApprovedBindings": 0,
        },
        "policy": {
            "recommendationIsNotHookApproval": True,
            "engineVerificationIsNotBindingVerification": True,
            "requiredBeforeRuntimeBinding": [
                "confirm owner/vtable slot or callsite", "confirm target build identity",
                "validate calling convention", "exercise in ForgeFSE probe",
            ],
        },
        "functions": functions,
        "verifiedEngineFunctions": verified_engine_functions,
    }
    outputs = args.output or [canonical_path]
    if not args.no_mirrors and args.output is None:
        outputs.extend(
            [
                Path(r"D:\Code\FableForge\docs\re_reference\fse_native_overlay.json"),
                Path(r"D:\Code\ForgeFSE\docs\fse_native_overlay.json"),
                Path(r"D:\Code\FQT\FQT\FSE_Source\docs\fse_native_overlay.json"),
            ]
        )
    changed = [str(path.resolve()) for path in outputs if write_if_changed(path.resolve(), payload)]
    print(json.dumps(payload["summary"], indent=2))
    print(f"changed={len(changed)}")
    for path in changed:
        print(f"  wrote {path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
