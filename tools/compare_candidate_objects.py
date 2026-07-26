#!/usr/bin/env python3
"""Compare compiled candidate .text sections with authoritative retail bytes."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import shutil
import subprocess
from datetime import datetime
from pathlib import Path


RELOCATION_LINE = re.compile(r"^([0-9a-fA-F]{8})\s+\S+\s+.+$")
DISASM_SYMBOL = re.compile(r"^\s*[0-9a-fA-F]+\s+<(.+)>:$")
DISASM_BYTES = re.compile(r"^\s*[0-9a-fA-F]+:\s+((?:[0-9a-fA-F]{2}\s+)+)")
CACHE_VERSION = 1


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def object_text(objdump: str, path: Path, expected_name: str) -> tuple[bytes, int, str]:
    process = subprocess.run([objdump, "-d", str(path)], capture_output=True, text=True, check=True)
    functions: list[dict[str, object]] = []
    section_index = -1
    current: dict[str, object] | None = None
    for line in process.stdout.splitlines():
        if line.startswith("Disassembly of section .text:"):
            section_index += 1
            current = None
            continue
        symbol = DISASM_SYMBOL.match(line)
        if symbol:
            current = {"symbol": symbol.group(1), "section": section_index, "bytes": bytearray()}
            functions.append(current)
            continue
        encoded = DISASM_BYTES.match(line)
        if encoded and current is not None:
            current["bytes"].extend(bytes.fromhex(encoded.group(1)))
    if not functions:
        raise RuntimeError(f"no disassembled functions in {path}")
    leaf = expected_name.rsplit("::", 1)[-1].lstrip("~")
    named = [item for item in functions if leaf and leaf in str(item["symbol"])]
    selected = max(named or functions, key=lambda item: len(item["bytes"]))
    return bytes(selected["bytes"]), int(selected["section"]), str(selected["symbol"])


def object_relocations(objdump: str, path: Path, selected_section: int) -> list[int]:
    section_process = subprocess.run([objdump, "-h", str(path)], capture_output=True, text=True, check=True)
    text_has_relocations: list[bool] = []
    header_lines = section_process.stdout.splitlines()
    for index, line in enumerate(header_lines):
        if re.match(r"^\s*\d+\s+\.text\s+", line):
            flags = header_lines[index + 1] if index + 1 < len(header_lines) else ""
            text_has_relocations.append("RELOC" in flags)
    if selected_section >= len(text_has_relocations) or not text_has_relocations[selected_section]:
        return []
    relocation_block_index = sum(text_has_relocations[: selected_section + 1]) - 1

    process = subprocess.run([objdump, "-r", str(path)], capture_output=True, text=True, check=True)
    blocks: list[list[int]] = []
    in_text = False
    text_section = -1
    for line in process.stdout.splitlines():
        if line.startswith("RELOCATION RECORDS FOR ["):
            in_text = "[.text]" in line
            if in_text:
                text_section += 1
                blocks.append([])
            continue
        if not in_text:
            continue
        match = RELOCATION_LINE.match(line.strip())
        if match:
            blocks[text_section].append(int(match.group(1), 16))
    return blocks[relocation_block_index] if relocation_block_index < len(blocks) else []


def mask_relocations(payload: bytes, offsets: list[int]) -> bytes:
    result = bytearray(payload)
    for offset in offsets:
        for index in range(offset, min(offset + 4, len(result))):
            result[index] = 0
    return bytes(result)


def write_tsv(path: Path, rows: list[dict[str, object]]) -> None:
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
    parser.add_argument("--objdump", default="objdump")
    parser.add_argument("--force", action="store_true", help="ignore the local object parity cache")
    args = parser.parse_args()
    root = args.root.resolve()
    gate = root / "rebuild" / "compile-gate"
    compiled_path = gate / "vc71-compiled.tsv"
    oracle_path = root / "rebuild" / "oracles" / "auto-re-candidates.tsv"
    objdump = shutil.which(args.objdump)
    if objdump is None:
        raise SystemExit(f"objdump not found: {args.objdump}")
    compiled = read_tsv(compiled_path)
    oracles = {row["address"].lower(): row for row in read_tsv(oracle_path)}
    cache_path = root / "rebuild" / "build" / "local-parity" / "retail-parity-cache.json"
    cache: dict[str, object] = {}
    if not args.force and cache_path.exists():
        try:
            loaded = json.loads(cache_path.read_text(encoding="utf-8"))
            if loaded.get("version") == CACHE_VERSION:
                cache = loaded.get("functions", {})
        except (OSError, ValueError):
            cache = {}
    # Bootstrap the local cache from an atomic canonical report when that report
    # is newer than both the object and the complete oracle ledger. This avoids
    # one redundant full disassembly immediately after upgrading the tool.
    prior_report_path = gate / "retail-parity.tsv"
    prior_rows = (
        {row["address"].lower(): row for row in read_tsv(prior_report_path)}
        if not args.force and not cache and prior_report_path.exists()
        else {}
    )
    prior_report_mtime_ns = (
        prior_report_path.stat().st_mtime_ns if prior_rows else 0
    )
    oracle_mtime_ns = oracle_path.stat().st_mtime_ns
    next_cache: dict[str, object] = {}
    cache_reused = 0
    rows: list[dict[str, object]] = []
    for item in compiled:
        if item.get("status") != "PASS":
            continue
        address = item["address"].lower()
        object_path = Path(item["object"])
        oracle = oracles.get(address)
        object_stat = object_path.stat()
        oracle_sha256 = (
            hashlib.sha256(bytes.fromhex(oracle["bytes"])).hexdigest() if oracle else ""
        )
        fingerprint = {
            "object_size": object_stat.st_size,
            "object_mtime_ns": object_stat.st_mtime_ns,
            "oracle_sha256": oracle_sha256,
            "oracle_name": oracle["name"] if oracle else "",
        }
        cached = cache.get(address)
        if cached and cached.get("fingerprint") == fingerprint:
            row = dict(cached["row"])
            row["module"] = item["module"]
            rows.append(row)
            next_cache[address] = {"fingerprint": fingerprint, "row": row}
            cache_reused += 1
            continue
        prior = prior_rows.get(address)
        if (
            prior
            and prior_report_mtime_ns >= object_stat.st_mtime_ns
            and prior_report_mtime_ns >= oracle_mtime_ns
        ):
            row = dict(prior)
            row["module"] = item["module"]
            rows.append(row)
            next_cache[address] = {"fingerprint": fingerprint, "row": row}
            cache_reused += 1
            continue
        if oracle is None:
            built, _, selected_symbol = object_text(objdump, object_path, "")
            row = {
                    "address": address,
                    "module": item["module"],
                    "status": "ORACLE_MISSING",
                    "retail_bytes": "",
                    "object_text_bytes": len(built),
                    "object_symbol": selected_symbol,
                    "first_difference": "",
                    "matching_prefix_bytes": "",
                    "relocation_count": "",
                    "relocation_offsets": "",
                    "retail_sha256": "",
                    "object_sha256": hashlib.sha256(built).hexdigest(),
                }
            rows.append(row)
            next_cache[address] = {"fingerprint": fingerprint, "row": row}
            continue
        retail = bytes.fromhex(oracle["bytes"])
        built, selected_section, selected_symbol = object_text(
            objdump, object_path, oracle["name"]
        )
        relocations = object_relocations(objdump, object_path, selected_section)
        normalized_retail = mask_relocations(retail, relocations)
        normalized_built = mask_relocations(built, relocations)
        common = min(len(retail), len(built))
        first_diff = next((index for index in range(common) if retail[index] != built[index]), common)
        exact = retail == built
        relocation_match = normalized_retail == normalized_built
        comparison_status = "MATCH" if exact else "RELOCATION_MATCH" if relocation_match else "DIFFER"
        row = {
                "address": address,
                "module": item["module"],
                "status": comparison_status,
                "retail_bytes": len(retail),
                "object_text_bytes": len(built),
                "object_symbol": selected_symbol,
                "first_difference": "" if exact else first_diff,
                "matching_prefix_bytes": first_diff,
                "relocation_count": len(relocations),
                "relocation_offsets": ";".join(f"0x{offset:x}" for offset in relocations),
                "retail_sha256": hashlib.sha256(retail).hexdigest(),
                "object_sha256": hashlib.sha256(built).hexdigest(),
            }
        rows.append(row)
        next_cache[address] = {"fingerprint": fingerprint, "row": row}
    write_tsv(gate / "retail-parity.tsv", rows)
    cache_path.parent.mkdir(parents=True, exist_ok=True)
    cache_temp = cache_path.with_suffix(".json.tmp")
    cache_temp.write_text(
        json.dumps({"version": CACHE_VERSION, "functions": next_cache}) + "\n",
        encoding="utf-8",
    )
    cache_temp.replace(cache_path)
    summary = {
        "generated_at": datetime.now().astimezone().isoformat(timespec="seconds"),
        "compared": len(rows),
        "matching": sum(row["status"] == "MATCH" for row in rows),
        "relocation_matching": sum(row["status"] == "RELOCATION_MATCH" for row in rows),
        "differing": sum(row["status"] == "DIFFER" for row in rows),
        "oracle_missing": sum(row["status"] == "ORACLE_MISSING" for row in rows),
    }
    temp = gate / "retail-parity.json.tmp"
    temp.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    temp.replace(gate / "retail-parity.json")
    print(json.dumps(summary, indent=2))
    print(f"parity_cache reused={cache_reused} recomputed={len(rows) - cache_reused}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
