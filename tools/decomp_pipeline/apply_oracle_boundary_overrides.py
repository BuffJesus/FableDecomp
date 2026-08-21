#!/usr/bin/env python3
"""Apply reviewed retail-function boundary corrections to an oracle TSV.

Ghidra occasionally exports alignment or the next function as part of a candidate. The
override ledger records both the exported and corrected lengths so stale or changed input is
rejected rather than silently truncated.

Usage: python apply_oracle_boundary_overrides.py <oracle.tsv> [--write]
"""
import csv
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DEFAULT_OVERRIDES = ROOT / "rebuild/oracles/boundary-overrides.tsv"


def read_rows(path):
    return list(csv.DictReader(open(path, encoding="utf-8-sig"), delimiter="\t"))


def main():
    if len(sys.argv) < 2:
        raise SystemExit("oracle TSV path required")
    oracle_path = Path(sys.argv[1])
    write = "--write" in sys.argv[2:]
    rows = read_rows(oracle_path)
    by_address = {row["address"].lower(): row for row in rows}
    overrides = read_rows(DEFAULT_OVERRIDES)
    changed = 0

    for override in overrides:
        address = override["address"].lower()
        row = by_address.get(address)
        if row is None:
            raise SystemExit(f"BOUNDARY OVERRIDE MISSING oracle row {address}")
        exported = int(override["exported_length"])
        corrected = int(override["corrected_length"])
        actual = bytes.fromhex(row["bytes"])
        if len(actual) == corrected:
            continue
        if len(actual) != exported:
            raise SystemExit(
                f"BOUNDARY OVERRIDE STALE {address}: expected {exported} or {corrected}, "
                f"found {len(actual)}"
            )
        if corrected >= exported:
            raise SystemExit(f"BOUNDARY OVERRIDE INVALID {address}: correction does not shorten")
        row["bytes"] = actual[:corrected].hex()
        row["length"] = str(corrected)
        changed += 1
        print(f"BOUNDARY TRIM {address} {exported}->{corrected}")

    if write and changed:
        with open(oracle_path, "w", encoding="utf-8", newline="") as stream:
            writer = csv.DictWriter(
                stream,
                fieldnames=["address", "name", "length", "bytes"],
                delimiter="\t",
                lineterminator="\n",
            )
            writer.writeheader()
            writer.writerows(rows)
    print(f"BOUNDARY OVERRIDES PASS rows={len(overrides)} changed={changed} write={write}")


if __name__ == "__main__":
    main()
