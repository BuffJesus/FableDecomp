#!/usr/bin/env python3
"""Validate the recovered retail CFrontEndManager::Init2 manifest."""

from __future__ import annotations

import argparse
import csv
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import parse_frontend  # noqa: E402


COMPONENT_RE = re.compile(
    r'CCharString::CCharString\([^\n]*,"([A-Z0-9_]+)",-1\);'
)
KEY_RE = re.compile(r"0x[0-9a-fA-F]+|0xffffffff")


def read_rows(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def key_forms(value: str) -> set[str]:
    if value == "-1":
        return {"0xffffffff"}
    number = int(value, 0)
    return {f"0x{number:x}", f"0x{number:02x}"}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--retail-root", default=parse_frontend.DEFAULT_ROOT)
    args = parser.parse_args()
    root = args.root.resolve()
    source = root / "work" / "frontend_re" / "manager_init_chain.c"
    manifest = root / "work" / "frontend_re" / "manager_init2_used_key_manifest.tsv"
    resource_manifest = root / "work" / "frontend_re" / "manager_init2_resource_manifest.tsv"
    children_manifest = root / "work" / "frontend_re" / "manager_init2_screen_children.tsv"
    rows = read_rows(manifest)
    resource_rows = read_rows(resource_manifest)
    children_rows = read_rows(children_manifest)
    text = source.read_text(encoding="utf-8")
    init2 = text[text.index("CFrontEndManager::Init2 @"):]

    errors: list[str] = []
    if len(rows) != 24:
        errors.append(f"expected 24 rows, found {len(rows)}")
    if len(resource_rows) != len(rows):
        errors.append(
            f"resource manifest row count {len(resource_rows)} != {len(rows)}"
        )
    if len(children_rows) != len(rows):
        errors.append(
            f"children manifest row count {len(children_rows)} != {len(rows)}"
        )
    expected_orders = list(range(1, len(rows) + 1))
    actual_orders = [int(row["order"]) for row in rows]
    if actual_orders != expected_orders:
        errors.append("order column is not contiguous")

    manifest_names = [row["definition_name"] for row in rows]
    source_names: list[str] = []
    for match in re.finditer(r'"([A-Z0-9_]+)"', init2):
        name = match.group(1)
        if name in manifest_names and name not in source_names:
            source_names.append(name)
    if source_names != manifest_names:
        errors.append("manifest names/order do not match the Init2 decomp")

    for row in rows:
        name = row["definition_name"]
        position = init2.find(f'"{name}",-1')
        if position < 0:
            errors.append(f"missing source literal: {name}")
            continue
        expected_keys = key_forms(row["used_key"])
        if row["relationship"] != "screen_child":
            end = init2.find("CreateComponent", position)
            evidence = init2[position : end if end >= 0 else position + 1200]
        else:
            evidence = init2[max(0, position - 800) : position]
        found = {token.lower() for token in KEY_RE.findall(evidence)}
        if not any(key.lower() in found for key in expected_keys):
            errors.append(f"used-key evidence not found near {name}: {row['used_key']}")

    try:
        entries, schema, _ = parse_frontend.load_all(
            args.retail_root, parse_frontend.DEFAULT_SCHEMA
        )
        by_name = {entry["name"]: entry for entry in entries}
        for row in resource_rows:
            entry = by_name.get(row["definition_name"])
            if entry is None:
                errors.append(f"retail frontend.bin entry missing: {row['definition_name']}")
                continue
            actual = {
                "frontend_bin_index": str(entry["index"]),
                "payload_bytes": str(len(entry["payload"])),
                "definition_type": entry["definition"],
                "index_in_definition": str(entry["index_in_definition"]),
            }
            for field, value in actual.items():
                if row[field] != value:
                    errors.append(
                        f"retail resource drift {row['definition_name']} "
                        f"{field}: manifest={row[field]} actual={value}"
                    )
        for row in children_rows:
            entry = by_name.get(row["definition_name"])
            if entry is None:
                errors.append(f"retail child entry missing: {row['definition_name']}")
                continue
            decoded = parse_frontend.decode_entry(entry, schema)[0]
            actual_children = ",".join(
                str(child) for child in decoded.get("Children", [])
            )
            if actual_children != row["children"]:
                errors.append(
                    f"retail child drift {row['definition_name']}: "
                    f"manifest={row['children']} actual={actual_children}"
                )
    except (OSError, KeyError, ValueError) as error:
        errors.append(f"retail frontend.bin audit unavailable: {error}")

    if errors:
        for error in errors:
            print(f"FRONTEND_INIT2_MANIFEST FAIL {error}")
        return 1
    print(
        "FRONTEND_INIT2_MANIFEST PASS "
        f"rows={len(rows)} source_names={len(source_names)} "
        f"retail_resources={len(resource_rows)} "
        f"screen_children={len(children_rows)}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
