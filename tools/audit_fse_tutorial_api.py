#!/usr/bin/env python3
"""Compare AlbionSecrets' tutorial API header with the normalized FSE manifest."""
from __future__ import annotations

import argparse
import collections
import json
import re
from pathlib import Path


DECLARATION = re.compile(
    r"^\s*.+?\s*(?P<name>[A-Za-z_]\w*)\s*\([^;]*\)\s*;\s*$"
)


def header_names(path: Path) -> list[str]:
    names: list[str] = []
    pending = ""
    for raw_line in path.read_text(encoding="utf-8-sig", errors="replace").splitlines():
        line = raw_line.split("//", 1)[0].strip()
        if not line:
            continue
        pending = f"{pending} {line}".strip()
        if ";" not in line:
            continue
        match = DECLARATION.match(pending)
        if match:
            names.append(match.group("name"))
        pending = ""
    return names


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("header", type=Path, help="ALL-INTERFACE-FUNCTIONS-FOR-FSE.h")
    parser.add_argument(
        "--manifest",
        type=Path,
        default=Path(__file__).resolve().parents[1] / "refs" / "fse_api_manifest.json",
    )
    args = parser.parse_args()

    declarations = header_names(args.header)
    counts = collections.Counter(declarations)
    manifest = json.loads(args.manifest.read_text(encoding="utf-8-sig"))
    records = manifest["functions"] if isinstance(manifest, dict) else manifest
    manifest_names = {record["name"] for record in records}
    header_set = set(declarations)

    print(f"header declarations: {len(declarations)}")
    print(f"header unique names: {len(header_set)}")
    print(f"manifest records: {len(records)}")
    print("header-only:", ", ".join(sorted(header_set - manifest_names)) or "none")
    print("manifest-only:", ", ".join(sorted(manifest_names - header_set)) or "none")
    overloads = [f"{name} ({count})" for name, count in sorted(counts.items()) if count > 1]
    print("overloaded names:", ", ".join(overloads) or "none")
    return 1 if header_set != manifest_names else 0


if __name__ == "__main__":
    raise SystemExit(main())
