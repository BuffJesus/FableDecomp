#!/usr/bin/env python3
"""Audit ForgeFSE's byte-colour bridge against the retail/PDB BGRA ABI."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


EXPECTED_FIELDS = ("B", "G", "R", "A")


def pdb_colour_fields(layout_text: str) -> tuple[str, ...]:
    rows: list[tuple[int, str]] = []
    for line in layout_text.splitlines():
        columns = line.split("\t")
        if len(columns) >= 5 and columns[0] == "CRGBColour_u_0_s_0" and columns[3] == "uchar":
            rows.append((int(columns[2]), columns[4]))
    return tuple(name for _, name in sorted(rows))


def forge_colour_fields(header_text: str) -> tuple[str, ...]:
    match = re.search(r"struct\s+CRGBColour\s*\{(?P<body>.*?)\};", header_text, re.S)
    if not match:
        return ()
    declaration = re.search(r"unsigned\s+char\s+([^;]+);", match.group("body"))
    if not declaration:
        return ()
    return tuple(part.strip() for part in declaration.group(1).split(","))


def parser_emits_bgra(source_text: str) -> bool:
    match = re.search(
        r"static\s+CRGBColour\s+ParseColorTable\s*\([^)]*\)\s*\{(?P<body>.*?)\n\}",
        source_text,
        re.S,
    )
    return bool(match and re.search(r"return\s*\{\s*b\s*,\s*g\s*,\s*r\s*,\s*a\s*\}\s*;", match.group("body")))


def audit(layout: Path, forge_root: Path) -> dict[str, object]:
    pdb_fields = pdb_colour_fields(layout.read_text(encoding="utf-8"))
    forge_fields = forge_colour_fields(
        (forge_root / "FableScriptExtender" / "GameInterface.h").read_text(encoding="utf-8")
    )
    parser_ok = parser_emits_bgra(
        (forge_root / "FableScriptExtender" / "LuaQuestState.cpp").read_text(encoding="utf-8")
    )
    checks = {
        "pdbLayoutIsBGRA": pdb_fields == EXPECTED_FIELDS,
        "forgeLayoutIsBGRA": forge_fields == EXPECTED_FIELDS,
        "luaParserEmitsBGRA": parser_ok,
    }
    return {
        "schema": "forgefse-colour-abi-audit/0.1",
        "ok": all(checks.values()),
        "checks": checks,
        "pdbFields": pdb_fields,
        "forgeFields": forge_fields,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--forge-root", type=Path, required=True)
    parser.add_argument("--layouts", type=Path, default=Path("ghidra_out/struct_layouts_egor.tsv"))
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = audit(args.layouts, args.forge_root)
    rendered = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(rendered, encoding="utf-8")
    print(rendered, end="")
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
