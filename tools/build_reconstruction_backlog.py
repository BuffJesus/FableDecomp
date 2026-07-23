#!/usr/bin/env python3
"""Prioritize module and function work that closes the C++ reconstruction gap."""
from __future__ import annotations

import argparse
import csv
from collections import defaultdict
from datetime import datetime
from pathlib import Path


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path: Path, rows: list[dict[str, object]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    fields = list(rows[0]) if rows else []
    temp = path.with_suffix(path.suffix + ".tmp")
    with temp.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, delimiter="\t", fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    temp.replace(path)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--function-limit", type=int, default=500)
    args = parser.parse_args()
    root = args.root.resolve()
    rebuild = root / "rebuild"
    functions = read_tsv(rebuild / "manifest" / "functions.tsv")
    gate_path = rebuild / "compile-gate" / "candidates.tsv"
    gate = {row["address"]: row for row in read_tsv(gate_path)} if gate_path.exists() else {}
    compiled_path = rebuild / "compile-gate" / "vc71-compiled.tsv"
    compiled = {
        row["address"].lower()
        for row in read_tsv(compiled_path)
        if row.get("status") == "PASS"
    } if compiled_path.exists() else set()
    parity_path = rebuild / "compile-gate" / "retail-parity.tsv"
    parity = {row["address"].lower(): row.get("status", "") for row in read_tsv(parity_path)} if parity_path.exists() else {}
    audit_path = rebuild / "compile-gate" / "signature-audit.tsv"
    audit = {row["address"].lower(): row for row in read_tsv(audit_path)} if audit_path.exists() else {}
    oracle_path = rebuild / "oracles" / "auto-re-candidates.tsv"
    oracles = {row["address"].lower(): row for row in read_tsv(oracle_path)} if oracle_path.exists() else {}

    grouped: dict[str, list[dict[str, str]]] = defaultdict(list)
    for row in functions:
        grouped[row["module"]].append(row)

    module_rows: list[dict[str, object]] = []
    module_meta: dict[str, dict[str, int | str]] = {}
    for module, rows in grouped.items():
        total = len(rows)
        cc_unknown = sum(row["calling_convention"] in ("", "unknown") for row in rows)
        proto_gap = sum(row["prototype_complete"] != "1" for row in rows)
        agents = sum(bool(row["agent_source_path"]) for row in rows)
        lifts = sum(bool(row["lift_grade"]) for row in rows)
        review = sum(row["naming_quality"].startswith("review-") for row in rows)
        if agents:
            track = "candidate-cluster"
            score = 400_000 + agents * 2_000 + lifts * 500 - proto_gap
        elif lifts:
            track = "proof-extension"
            score = 300_000 + lifts * 2_000 - proto_gap * 5
        elif module != "_global" and 4 <= total and 0 < proto_gap <= 5:
            track = "quick-closure"
            score = 200_000 + total * 10 - proto_gap * 1_000
        else:
            track = "bulk-prototype"
            score = 100_000 + proto_gap * 10 + cc_unknown
        closure = total - proto_gap
        module_meta[module] = {"track": track, "score": score, "lifts": lifts, "agents": agents}
        module_rows.append(
            {
                "rank": 0,
                "track": track,
                "module": module,
                "score": score,
                "catalog_functions": total,
                "prototype_complete": closure,
                "prototype_gaps": proto_gap,
                "calling_convention_unknown": cc_unknown,
                "naming_review": review,
                "agent_candidates": agents,
                "verified_lifts": lifts,
            }
        )
    module_rows.sort(key=lambda row: (-int(row["score"]), str(row["module"]).casefold()))
    for rank, row in enumerate(module_rows, 1):
        row["rank"] = rank

    function_rows: list[dict[str, object]] = []
    for row in functions:
        cc_gap = row["calling_convention"] in ("", "unknown")
        proto_gap = row["prototype_complete"] != "1"
        compile_row = gate.get(row["address"], {})
        if row["address"] in compiled and parity.get(row["address"]) in ("MATCH", "RELOCATION_MATCH"):
            continue
        if not (cc_gap or proto_gap or compile_row):
            continue
        meta = module_meta[row["module"]]
        score = int(meta["score"])
        if row["address"] in compiled:
            score += 1_100_000
        elif audit.get(row["address"], {}).get("status") == "REVIEW":
            score += 1_200_000
        elif compile_row:
            score += 1_000_000
        if row["naming_quality"] == "accepted":
            score += 200
        if row["clean_decompile"] == "yes":
            score += 100
        if parity.get(row["address"]) in ("MATCH", "RELOCATION_MATCH"):
            gap = "retail-match-needs-promotion"
        elif row["address"] in compiled:
            gap = "compiled-needs-parity"
        elif audit.get(row["address"], {}).get("status") == "REVIEW":
            gap = "signature-audit"
        elif cc_gap and proto_gap:
            gap = "calling-convention+prototype"
        elif cc_gap:
            gap = "calling-convention"
        elif proto_gap:
            gap = "prototype"
        else:
            gap = "candidate-compile"
        function_rows.append(
            {
                "rank": 0,
                "address": row["address"],
                "module": row["module"],
                "name": row["name"],
                "workstream": meta["track"],
                "gap": gap,
                "score": score,
                "calling_convention": row["calling_convention"],
                "return_type": row["return_type"],
                "parameter_types": row["parameter_types"],
                "checker_verdict": row["agent_verdict"],
                "compile_blocker": "" if row["address"] in compiled else compile_row.get("first_blocker", ""),
                "retail_parity": parity.get(row["address"], ""),
                "signature_issues": audit.get(row["address"], {}).get("issues", ""),
                "missing_dependencies": compile_row.get("missing_dependencies", ""),
            }
        )
    function_rows.sort(key=lambda row: (-int(row["score"]), str(row["address"])))
    function_rows = function_rows[: args.function_limit]
    for rank, row in enumerate(function_rows, 1):
        row["rank"] = rank

    backlog = rebuild / "backlog"
    write_tsv(backlog / "modules.tsv", module_rows)
    write_tsv(backlog / "functions.tsv", function_rows)
    quick_wins: list[dict[str, object]] = []
    for address, candidate in gate.items():
        if address in compiled:
            continue
        oracle = oracles.get(address, {})
        quick_wins.append(
            {
                "rank": 0,
                "address": address,
                "module": candidate.get("module", ""),
                "name": candidate.get("name", ""),
                "retail_bytes": int(oracle.get("length", "0") or 0),
                "candidate_source_bytes": int(candidate.get("bytes", "0") or 0),
                "checker_verdict": candidate.get("checker_verdict", ""),
                "first_blocker": candidate.get("first_blocker", ""),
            }
        )
    quick_wins.sort(
        key=lambda row: (
            int(row["retail_bytes"]) if int(row["retail_bytes"]) else 1 << 30,
            int(row["candidate_source_bytes"]),
            str(row["address"]),
        )
    )
    for rank, row in enumerate(quick_wins, 1):
        row["rank"] = rank
    write_tsv(backlog / "candidate-quick-wins.tsv", quick_wins)
    timestamp = datetime.now().astimezone().isoformat(timespec="seconds")
    lines = [
        "# Reconstruction backlog",
        "",
        f"Generated: `{timestamp}`",
        "",
        "The order favors generated candidate clusters first, then extension of proven modules, small prototype closures, and finally bulk signature work.",
        "",
        "## Highest-value modules",
        "",
        "| Rank | Track | Module | Functions | Prototype gaps | Agent candidates | Verified lifts |",
        "|---:|---|---|---:|---:|---:|---:|",
    ]
    for row in module_rows[:25]:
        lines.append(
            f"| {row['rank']} | {row['track']} | `{row['module']}` | {row['catalog_functions']} | {row['prototype_gaps']} | {row['agent_candidates']} | {row['verified_lifts']} |"
        )
    lines.extend(
        [
            "",
            "## Small candidate quick wins",
            "",
            "Uncompiled auto-RE candidates ordered by authoritative retail function size, then candidate source size.",
            "",
            "| Rank | Address | Module | Retail bytes | Candidate bytes | Blocker |",
            "|---:|---|---|---:|---:|---|",
        ]
    )
    for row in quick_wins[:15]:
        blocker = str(row["first_blocker"]).replace("|", "\\|")
        lines.append(
            f"| {row['rank']} | `{row['address']}` | `{row['module']}` | {row['retail_bytes']} | {row['candidate_source_bytes']} | {blocker} |"
        )
    lines.extend(
        [
            "",
            "## First function queue",
            "",
            "| Rank | Address | Module | Gap | Compiler blocker |",
            "|---:|---|---|---|---|",
        ]
    )
    for row in function_rows[:40]:
        blocker = str(row["compile_blocker"]).replace("|", "\\|")
        lines.append(f"| {row['rank']} | `{row['address']}` | `{row['module']}` | {row['gap']} | {blocker} |")
    lines.append("")
    temp = backlog / "README.md.tmp"
    temp.write_text("\n".join(lines), encoding="utf-8")
    temp.replace(backlog / "README.md")
    print(f"modules={len(module_rows)} queued_functions={len(function_rows)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
