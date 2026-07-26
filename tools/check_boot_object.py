#!/usr/bin/env python3
"""Compare a boot-spine COFF object with its checked-in retail oracle."""

from __future__ import annotations

import argparse
import csv
import shutil
from pathlib import Path

from compare_candidate_objects import (
    mask_relocations,
    object_relocations,
    object_text,
)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--object", required=True, type=Path)
    parser.add_argument("--address", required=True)
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
    )
    parser.add_argument("--objdump", default="objdump")
    args = parser.parse_args()

    root = args.root.resolve()
    address = args.address.lower().removeprefix("0x")
    oracle_path = root / "rebuild/integration/boot_oracles.tsv"
    with oracle_path.open("r", encoding="utf-8-sig", newline="") as stream:
        oracles = {
            row["address"].lower(): row
            for row in csv.DictReader(stream, delimiter="\t")
        }

    oracle = oracles.get(address)
    if oracle is None:
        raise SystemExit(f"BOOT_OBJECT FAIL address={address} reason=oracle-missing")

    objdump = shutil.which(args.objdump)
    if objdump is None:
        raise SystemExit(f"BOOT_OBJECT FAIL address={address} reason=objdump-missing")

    retail = bytes.fromhex(oracle["bytes"])
    built, section, symbol = object_text(
        objdump,
        args.object.resolve(),
        oracle["name"],
    )
    relocations = object_relocations(objdump, args.object.resolve(), section)

    if len(retail) != len(built):
        raise SystemExit(
            f"BOOT_OBJECT FAIL address={address} "
            f"retail={len(retail)} built={len(built)}"
        )
    if mask_relocations(retail, relocations) != mask_relocations(built, relocations):
        raise SystemExit(
            f"BOOT_OBJECT FAIL address={address} "
            "reason=non-relocation-bytes-differ"
        )

    print(
        f"BOOT_OBJECT RELOCATION_MATCH address={address} symbol={symbol} "
        f"bytes={len(built)} relocations={len(relocations)}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
