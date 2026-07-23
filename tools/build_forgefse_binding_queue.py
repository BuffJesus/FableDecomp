#!/usr/bin/env python3
"""Build the non-executable ForgeFSE binding-verification queue."""
from __future__ import annotations

import argparse
import csv
import json
from collections import Counter
from pathlib import Path


def normalize_address(value: str) -> str:
    return f"0x{int(value, 16):08X}"


def write_tsv(path: Path, rows: list[dict[str, object]], fields: list[str]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def stage_for(candidate: dict[str, object], function: dict[str, object]) -> str:
    if bool(function.get("hookApproved")):
        return "hook-approved"
    if bool(candidate.get("engineImplementationVerified")):
        return "implementation-verified"
    if candidate.get("compiledStatus") == "PASS" and candidate.get("behaviorTest") == "PASS":
        return "compiled-behavior-pass"
    if candidate.get("agentVerdict") == "PASS":
        return "agent-reviewed"
    return "needs-reversal"


def compact_type(value: str) -> str:
    return "".join(value.replace("std::", "std::").split())


def bridge_kind(function: dict[str, object], candidate: dict[str, object]) -> tuple[str, int]:
    """Classify wrapper work without claiming C++ wrapper types are engine ABI types."""
    fse_types = [
        str(item.get("type", "")) for item in function.get("parameters", [])
        if str(item.get("type", "")) != "sol::this_state"
    ]
    candidate_count = int(candidate.get("parameterCount", 0))
    candidate_text = str(candidate.get("parameterTypesText", ""))
    candidate_types = [item.strip() for item in candidate_text.split(",") if item.strip()]

    # Retail member prototypes exported from Ghidra include the explicit owner
    # pointer. Entity wrappers already carry pMe; Quest wrappers do not.
    expected_count = len(fse_types)
    if function.get("scope") == "Quest" and candidate_types and compact_type(candidate_types[0]).endswith("*"):
        expected_count += 1
    if candidate_count != expected_count:
        return "signature-review", 0

    comparable_candidate_types = candidate_types
    if function.get("scope") == "Quest" and len(candidate_types) == len(fse_types) + 1:
        comparable_candidate_types = candidate_types[1:]
    exact_parameters = [compact_type(item) for item in fse_types] == [
        compact_type(item) for item in comparable_candidate_types
    ]
    exact_return = compact_type(str(function.get("returnType", ""))) == compact_type(
        str(candidate.get("returnType", ""))
    )
    if exact_parameters and exact_return:
        return "direct-signature", 1
    return "wrapper-adapter", 1


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--overlay", type=Path)
    parser.add_argument("--tsv", type=Path)
    parser.add_argument("--markdown", type=Path)
    args = parser.parse_args()

    root = args.root.resolve()
    overlay_path = (args.overlay or root / "rebuild" / "sdk" / "fse_native_overlay.json").resolve()
    tsv_path = (args.tsv or root / "rebuild" / "backlog" / "forgefse-binding-queue.tsv").resolve()
    markdown_path = (args.markdown or root / "rebuild" / "backlog" / "FORGEFSE_BINDING_QUEUE.md").resolve()
    overlay = json.loads(overlay_path.read_text(encoding="utf-8-sig"))

    if not str(overlay.get("schemaVersion", "")).startswith("1."):
        raise SystemExit("unsupported FSE native-overlay schema")
    policy = overlay.get("policy", {})
    if not policy.get("recommendationIsNotHookApproval") or not policy.get(
        "engineVerificationIsNotBindingVerification"
    ):
        raise SystemExit("native overlay does not preserve the required safety policy")

    by_address: dict[str, dict[str, object]] = {}
    recommendations = 0
    for function in overlay.get("functions", []):
        recommended = function.get("recommendedAddress")
        if not recommended:
            continue
        recommendations += 1
        address = normalize_address(str(recommended))
        candidates = [
            item for item in function.get("candidates", [])
            if normalize_address(str(item.get("address", "0"))) == address
        ]
        if len(candidates) != 1:
            raise SystemExit(
                f"{function.get('scope')}::{function.get('name')} recommendation has "
                f"{len(candidates)} matching evidence candidates"
            )
        candidate = candidates[0]
        bridge, arity_compatible = bridge_kind(function, candidate)
        api_name = f"{function.get('scope', '')}.{function.get('name', '')}"
        existing = by_address.get(address)
        if existing is not None:
            existing["api_names"] = f"{existing['api_names']};{api_name}"
            continue
        by_address[address] = {
            "address": address,
            "api_names": api_name,
            "qualified_name": candidate.get("qualifiedName", ""),
            "module": candidate.get("module", ""),
            "match_status": function.get("matchStatus", ""),
            "owner_relevance": candidate.get("ownerRelevance", ""),
            "prototype_complete": int(bool(candidate.get("prototypeComplete"))),
            "calling_convention": candidate.get("callingConvention", ""),
            "return_type": candidate.get("returnType", ""),
            "parameter_types": candidate.get("parameterTypesText", ""),
            "score": int(candidate.get("score", 0)),
            "bridge_kind": bridge,
            "arity_compatible": arity_compatible,
            "verification_stage": stage_for(candidate, function),
            "engine_implementation_verified": int(bool(candidate.get("engineImplementationVerified"))),
            "hook_approved": int(bool(function.get("hookApproved"))),
            "evidence": ";".join(str(item) for item in candidate.get("evidence", [])),
        }

    if recommendations != int(overlay.get("summary", {}).get("ownerAlignedRecommendations", -1)):
        raise SystemExit("owner-aligned recommendation summary is stale")

    owner_rank = {"strong": 0, "moderate": 1, "none": 2}
    stage_rank = {
        "needs-reversal": 0,
        "agent-reviewed": 1,
        "compiled-behavior-pass": 2,
        "implementation-verified": 3,
        "hook-approved": 4,
    }
    bridge_rank = {"direct-signature": 0, "wrapper-adapter": 1, "signature-review": 2}
    rows = sorted(
        by_address.values(),
        key=lambda row: (
            int(row["hook_approved"]),
            stage_rank.get(str(row["verification_stage"]), 9),
            bridge_rank.get(str(row["bridge_kind"]), 9),
            0 if row["match_status"] == "unique" else 1,
            owner_rank.get(str(row["owner_relevance"]), 9),
            -int(row["prototype_complete"]),
            -int(row["score"]),
            str(row["api_names"]).casefold(),
            str(row["address"]),
        ),
    )
    for rank, row in enumerate(rows, 1):
        row["rank"] = rank

    fields = [
        "rank", "address", "api_names", "qualified_name", "module", "match_status",
        "owner_relevance", "prototype_complete", "calling_convention", "return_type",
        "parameter_types", "score", "verification_stage", "engine_implementation_verified",
        "hook_approved", "bridge_kind", "arity_compatible", "evidence",
    ]
    write_tsv(tsv_path, rows, fields)

    stage_counts = Counter(str(row["verification_stage"]) for row in rows)
    bridge_counts = Counter(str(row["bridge_kind"]) for row in rows)
    lines = [
        "# ForgeFSE binding verification queue",
        "",
        "Generated from `rebuild/sdk/fse_native_overlay.json`. Rows are reverse-engineering and",
        "prototype-review targets, not executable bindings. The unattended agent may advance a row",
        "to structural review; it cannot set `hookApproved`.",
        "",
        f"- FSE recommendations: {recommendations}",
        f"- Unique retail target addresses: {len(rows)}",
        f"- Needs reversal: {stage_counts['needs-reversal']}",
        f"- Agent reviewed: {stage_counts['agent-reviewed']}",
        f"- Implementation verified: {stage_counts['implementation-verified']}",
        f"- Hook approved: {stage_counts['hook-approved']}",
        f"- Direct wrapper/retail signatures: {bridge_counts['direct-signature']}",
        f"- Wrapper adapters required: {bridge_counts['wrapper-adapter']}",
        f"- Signature/arity review required: {bridge_counts['signature-review']}",
        "",
        "| Rank | FSE API | Retail candidate | Address | Bridge | Match | Owner | Prototype | Stage | Score |",
        "|---:|---|---|---|---|---|---|:---:|---|---:|",
    ]
    for row in rows[:100]:
        def escape(value: object) -> str:
            return str(value).replace("|", "\\|")

        lines.append(
            f"| {row['rank']} | `{escape(row['api_names'])}` | `{escape(row['qualified_name'])}` | "
            f"`{row['address']}` | {row['bridge_kind']} | {row['match_status']} | {row['owner_relevance']} | "
            f"{'yes' if row['prototype_complete'] else 'no'} | {row['verification_stage']} | {row['score']} |"
        )
    markdown_path.parent.mkdir(parents=True, exist_ok=True)
    markdown_path.write_text("\n".join(lines) + "\n", encoding="utf-8")

    print(
        f"forgefse_bindings={len(rows)} needs_reversal={stage_counts['needs-reversal']} "
        f"agent_reviewed={stage_counts['agent-reviewed']} verified={stage_counts['implementation-verified']} "
        f"approved={stage_counts['hook-approved']}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
