#!/usr/bin/env python3
"""Build a typed native script catalog and correlate reconstructed seed ports."""

from __future__ import annotations

import argparse
import csv
import json
import re
from pathlib import Path
from typing import Any

try:
    from .recover_scripts import classify
except ImportError:
    from recover_scripts import classify


ALIASES = {
    "GuardianSisterInfo": "QS_GuardianSisterInfo",
    "GuardianSisterInfo2": "QS_GuardianSisterInfo2_SisterInBanditCamp",
    "GuardianTrophyDealerInfo": "QS_GuardianTrophyDealerInfo",
    "MeetSister": "QS_MeetSister",
    "MazeResearch": "V_MazeResearch",
    "HerosOldHouse": "Q_HerosOldHouse",
}


def address_from_allocator(value: str) -> str | None:
    match = re.search(r"(?:LAB_|0x)?([0-9a-fA-F]{8})", value)
    return f"0x{match.group(1).upper()}" if match else None


def load_registry(path: Path) -> list[dict[str, Any]]:
    rows = []
    with path.open(encoding="utf-8-sig", newline="") as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            name = row["quest_name"]
            rows.append({
                "name": name,
                "kind": classify(name, master=row.get("master") == "1"),
                "numericId": int(row["id"]),
                "master": row.get("master") == "1",
                "allocator": row.get("allocFunc") or None,
                "allocatorAddress": address_from_allocator(row.get("allocFunc", "")),
                "dataAllocator": row.get("dataAlloc") or None,
                "section": row.get("section") or None,
                "evidenceLevel": "registry-fact",
                "source": str(path),
            })
    return rows


def correlate(catalog: list[dict[str, Any]], corpus_path: Path | None) -> list[dict[str, Any]]:
    if not corpus_path:
        return []
    corpus = json.loads(corpus_path.read_text(encoding="utf-8-sig"))
    native = {row["name"]: row for row in catalog}
    results = []
    for package in corpus["packages"]:
        for registration in package["registry"]:
            candidate = ALIASES.get(registration["name"], registration["key"])
            matched = native.get(candidate)
            results.append({
                "package": package["name"],
                "reconstructedKey": registration["key"],
                "reconstructedName": registration["name"],
                "nativeName": candidate,
                "status": "matched" if matched else "unmatched",
                "nativeSection": matched["section"] if matched else None,
                "nativeAllocatorAddress": matched["allocatorAddress"] if matched else None,
                "identityEvidence": "curated-alias" if registration["name"] in ALIASES else "exact-name",
            })
    return results


def build(registry: Path, output: Path, corpus: Path | None = None) -> dict[str, int]:
    scripts = load_registry(registry.resolve())
    correlations = correlate(scripts, corpus.resolve() if corpus else None)
    by_kind: dict[str, int] = {}
    for row in scripts:
        by_kind[row["kind"]] = by_kind.get(row["kind"], 0) + 1
    payload = {
        "schema": "fable-native-script-catalog/0.1",
        "source": str(registry.resolve()),
        "scripts": scripts,
        "seedCorrelations": correlations,
        "summary": {"total": len(scripts), "byKind": dict(sorted(by_kind.items())),
                    "matchedSeeds": sum(row["status"] == "matched" for row in correlations)},
    }
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
    return {"scripts": len(scripts), "matchedSeeds": payload["summary"]["matchedSeeds"]}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--registry", type=Path, default=Path("ghidra_out/quest_registry_table.tsv"))
    parser.add_argument("--corpus", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(build(args.registry, args.output, args.corpus), sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

