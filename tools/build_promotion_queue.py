#!/usr/bin/env python3
"""Rank uncompiled auto-RE candidates for focused VC7.1 promotion work."""
from __future__ import annotations

import argparse
import csv
from datetime import datetime
from pathlib import Path


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def write_tsv(path: Path, rows: list[dict[str, object]]) -> None:
    fields = list(rows[0]) if rows else []
    temporary = path.with_suffix(path.suffix + ".tmp")
    with temporary.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    temporary.replace(path)


def list_count(value: str) -> int:
    return len([item for item in value.split(";") if item.strip()])


def feature_count(value: str) -> int:
    total = 0
    for item in value.split(";"):
        if not item.strip():
            continue
        try:
            total += int(item.rsplit(":", 1)[1])
        except (IndexError, ValueError):
            total += 1
    return total


def lane_for(row: dict[str, str], missing: int, incompatible: int) -> str:
    if row.get("compile_ready") == "1":
        return "compile-now"
    if row.get("host_cpp20_syntax") == "PASS" and missing <= 2:
        return "vc71-port"
    if missing == 0 and int(row.get("compiler_errors") or 0) <= 3:
        return "declaration-fix"
    if missing <= 3 and incompatible <= 12:
        return "dependency-stub"
    return "manual-lift"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--limit", type=int, default=100)
    args = parser.parse_args()
    root = args.root.resolve()
    rebuild = root / "rebuild"

    candidates = read_tsv(rebuild / "compile-gate" / "candidates.tsv")
    signatures = {
        row["address"]: row for row in read_tsv(rebuild / "compile-gate" / "signature-audit.tsv")
    }
    compiled = {
        row["address"].lower()
        for row in read_tsv(rebuild / "compile-gate" / "vc71-compiled.tsv")
        if row.get("status") == "PASS" and row.get("behavior_test") == "PASS"
    }

    ranked: list[tuple[tuple[object, ...], dict[str, object]]] = []
    for candidate in candidates:
        address = candidate["address"].lower()
        if address in compiled:
            continue
        signature = signatures.get(address, {})
        missing = list_count(candidate.get("missing_dependencies", ""))
        incompatible = feature_count(candidate.get("vc71_features", ""))
        compiler_errors = int(candidate.get("compiler_errors") or 0)
        source_bytes = int(candidate.get("bytes") or 0)
        checker_pass = candidate.get("checker_verdict") == "PASS"
        signature_pass = signature.get("status") == "PASS"
        integrity_pass = candidate.get("integrity") == "PASS"
        lane = lane_for(candidate, missing, incompatible)
        lane_rank = {
            "compile-now": 0,
            "vc71-port": 1,
            "declaration-fix": 2,
            "dependency-stub": 3,
            "manual-lift": 4,
        }[lane]
        score = (
            0 if checker_pass else 1,
            0 if integrity_pass else 1,
            0 if signature_pass else 1,
            lane_rank,
            missing,
            incompatible,
            compiler_errors,
            source_bytes,
            address,
        )
        ranked.append(
            (
                score,
                {
                    "rank": 0,
                    "address": address,
                    "module": candidate["module"],
                    "name": candidate["name"],
                    "lane": lane,
                    "checker": candidate["checker_verdict"],
                    "signature": signature.get("status", "UNKNOWN"),
                    "host_syntax": candidate.get("host_cpp20_syntax", ""),
                    "compiler_errors": compiler_errors,
                    "missing_dependencies": missing,
                    "vc71_incompatibilities": incompatible,
                    "source_bytes": source_bytes,
                    "origin": candidate.get("origin", ""),
                    "first_blocker": candidate.get("first_blocker", ""),
                    "snapshot": candidate.get("snapshot", ""),
                },
            )
        )

    ranked.sort(key=lambda item: item[0])
    rows = [item[1] for item in ranked[: max(0, args.limit)]]
    for index, row in enumerate(rows, 1):
        row["rank"] = index

    backlog = rebuild / "backlog"
    backlog.mkdir(parents=True, exist_ok=True)
    write_tsv(backlog / "promotion_queue.tsv", rows)

    timestamp = datetime.now().astimezone().isoformat(timespec="seconds")
    lines = [
        "# Curated C++ promotion queue",
        "",
        f"Generated: `{timestamp}`",
        "",
        f"Uncompiled auto-RE candidates: **{len(ranked)}**. Showing: **{len(rows)}**.",
        "",
        "Ranking favors checker/integrity/signature PASS, then the smallest declaration, dependency, VC7.1, and source-size repair surface. It does not claim semantic correctness; every promotion still needs a focused behavior oracle and retail comparison.",
        "",
        "| Rank | Address | Owner/function | Lane | Signature | Missing deps | VC7.1 fixes | Source bytes | First blocker |",
        "|---:|---|---|---|---|---:|---:|---:|---|",
    ]
    for row in rows:
        owner = row["name"] if row["module"] == "_global" else f"{row['module']}::{row['name']}"
        blocker = str(row["first_blocker"]).replace("|", "\\|")
        lines.append(
            f"| {row['rank']} | `0x{str(row['address']).upper()}` | `{owner}` | `{row['lane']}` | "
            f"`{row['signature']}` | {row['missing_dependencies']} | {row['vc71_incompatibilities']} | "
            f"{row['source_bytes']} | {blocker} |"
        )
    lines.append("")
    temporary = backlog / "PROMOTION_QUEUE.md.tmp"
    temporary.write_text("\n".join(lines), encoding="utf-8")
    temporary.replace(backlog / "PROMOTION_QUEUE.md")
    print(f"promotion_candidates={len(ranked)} wrote={len(rows)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
