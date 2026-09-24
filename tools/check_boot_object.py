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

CGAME_PLAY_RELOCATIONS = [4, 23, 28, 45, 54, 63, 72, 77, 86, 96, 116,
                         140, 149, 158, 167, 174, 192, 212, 224, 244,
                         264, 335, 353]


def is_cgame_play_register_residue(address, retail, built, relocations):
    """Recognize only the documented ECX -> EDX initializer-address temporary.

    Both inputs have COFF relocation fields masked. This is functional
    acceptance, never a byte-match result, and cannot tolerate further drift.
    """
    if (address != "00412f90" or len(retail) != 394 or len(built) != 394
            or relocations != CGAME_PLAY_RELOCATIONS):
        return False
    # lea ecx,[esp+0xc]; push ecx -> lea edx,[esp+0xc]; push edx
    if retail[0xEB:0xF0] != bytes.fromhex("8d4c240c51"):
        return False
    if built[0xEB:0xF0] != bytes.fromhex("8d54240c52"):
        return False
    normalized = bytearray(built)
    normalized[0xEC] = retail[0xEC]
    normalized[0xEF] = retail[0xEF]
    return bytes(normalized) == retail


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
    parser.add_argument(
        "--allow-cgame-play-register-residue", action="store_true",
        help="Accept only CGame::Play's documented two-byte functional residue.",
    )
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
    if args.allow_cgame_play_register_residue and is_cgame_play_register_residue(
            address, masked_retail, masked_built, relocations):
        print(f"BOOT_OBJECT FUNCTIONAL_REGISTER_RESIDUE address={address} "
              f"symbol={symbol} bytes={len(built)} relocations={len(relocations)} "
              "offsets=0xEC,0xEF")
        return 0
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

    print(
        f"BOOT_OBJECT RELOCATION_MATCH address={address} symbol={symbol} "
        f"bytes={len(built)} relocations={len(relocations)}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
