#!/usr/bin/env python3
"""Compare a boot-spine COFF object with its checked-in retail oracle."""

from __future__ import annotations

import argparse
import csv
import shutil
import sys
from pathlib import Path

from compare_candidate_objects import (
    mask_relocations,
    object_relocations,
    object_text,
)
sys.path.insert(0, str(Path(__file__).resolve().parent / "decomp_pipeline"))
from gate_game_component_abi import Coff


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--object", required=True, type=Path)
    parser.add_argument("--address", required=True)
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
    )
    parser.add_argument(
        "--allow-move",
        metavar="SOURCE:LENGTH:DESTINATION",
        help=(
            "Accept one documented instruction-scheduling residue when moving "
            "the built byte range to DESTINATION reproduces retail."
        ),
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
        len(retail),
    )
    relocations = object_relocations(objdump, args.object.resolve(), section)

    if len(retail) != len(built):
        raise SystemExit(
            f"BOOT_OBJECT FAIL address={address} "
            f"retail={len(retail)} built={len(built)}"
        )
    masked_retail = mask_relocations(retail, relocations)
    masked_built = mask_relocations(built, relocations)
    if masked_retail != masked_built and args.allow_move:
        try:
            source_text, length_text, destination_text = args.allow_move.split(":")
            source = int(source_text, 0)
            length = int(length_text, 0)
            destination = int(destination_text, 0)
        except (TypeError, ValueError):
            raise SystemExit(
                "BOOT_OBJECT FAIL "
                f"address={address} reason=invalid-allow-move"
            )

        moved = masked_built[source : source + length]
        candidate = (
            masked_built[:source] +
            masked_built[source + length :]
        )
        candidate = (
            candidate[:destination] +
            moved +
            candidate[destination:]
        )
        if candidate == masked_retail:
            print(
                f"BOOT_OBJECT SCHEDULING_RESIDUE address={address} "
                f"symbol={symbol} bytes={len(built)} "
                f"relocations={len(relocations)} move={args.allow_move}"
            )
            return 0

    if masked_retail != masked_built:
        raise SystemExit(
            f"BOOT_OBJECT FAIL address={address} "
            "reason=non-relocation-bytes-differ"
        )

    # Masking an absolute switch-table entry also hides its destination. Two
    # language cases previously passed this gate with their targets swapped.
    # Same-section relocations have a known retail destination: this function's
    # address plus the target's offset from its entry point. External symbols
    # still need their own independently established address map.
    coff = Coff(args.object.resolve())
    coff_section, entry_offset = coff.symbols[symbol]
    section_data, section_relocations = coff.sections[coff_section]
    internal_targets = 0
    for offset, (target, kind) in section_relocations.items():
        relative = offset - entry_offset
        if kind != 6 or not 0 <= relative <= len(retail) - 4:
            continue
        location = coff.symbols.get(target)
        if location is None or location[0] != coff_section:
            continue
        addend = int.from_bytes(section_data[offset:offset + 4], "little")
        expected = int(address, 16) + location[1] - entry_offset + addend
        actual = int.from_bytes(retail[relative:relative + 4], "little")
        if actual != expected:
            raise SystemExit(
                f"BOOT_OBJECT FAIL address={address} reason=internal-relocation-target "
                f"offset={relative:x} retail={actual:08x} built={expected:08x}"
            )
        internal_targets += 1

    print(
        f"BOOT_OBJECT RELOCATION_MATCH address={address} symbol={symbol} "
        f"bytes={len(built)} relocations={len(relocations)} "
        f"internal_targets={internal_targets}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
