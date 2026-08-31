#!/usr/bin/env python3
"""Build an evidence-gated ForgeFSE shadow/override manifest."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Any


def build(catalog_path: Path, corpus_path: Path, output: Path, mode: str) -> dict[str, int]:
    if mode not in {"disabled", "shadow", "override"}:
        raise ValueError(f"unsupported mode {mode}")
    catalog = json.loads(catalog_path.read_text(encoding="utf-8-sig"))
    corpus = json.loads(corpus_path.read_text(encoding="utf-8-sig"))
    packages = {row["name"]: row for row in corpus["packages"]}
    entries: list[dict[str, Any]] = []
    for correlation in catalog.get("seedCorrelations", []):
        if correlation["status"] != "matched":
            continue
        package = packages[correlation["package"]]
        registration = package["registry"][0]
        evidence = package["evidenceLevel"]
        if mode == "override" and evidence != "verified-port":
            raise ValueError(
                f"refusing override for {correlation['nativeName']}: evidence is {evidence}, not verified-port"
            )
        entries.append({
            "nativeName": correlation["nativeName"],
            "nativeSection": correlation["nativeSection"],
            "package": package["name"],
            "luaFile": registration["file"],
            "mode": mode,
            "evidenceLevel": evidence,
            "archiveSha256": package["archiveSha256"],
            "mutatingCallsAllowed": mode == "override",
            "saveWritesAllowed": mode == "override",
        })
    entries.sort(key=lambda row: row["nativeName"])
    payload = {"schema": "forgefse-retail-script-overrides/0.1", "defaultMode": "disabled", "entries": entries}
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
    return {"entries": len(entries), "shadow": sum(row["mode"] == "shadow" for row in entries),
            "override": sum(row["mode"] == "override" for row in entries)}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--catalog", type=Path, required=True)
    parser.add_argument("--corpus", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--mode", choices=("disabled", "shadow", "override"), default="shadow")
    args = parser.parse_args()
    print(json.dumps(build(args.catalog.resolve(), args.corpus.resolve(), args.output.resolve(), args.mode), sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

