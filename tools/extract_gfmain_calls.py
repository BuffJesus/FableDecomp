#!/usr/bin/env python3
"""Extract GFMain's direct retail call edges without taking the Ghidra lock."""

from __future__ import annotations

import argparse
import csv
import re
import shutil
import subprocess
from pathlib import Path


INSTRUCTION_CALL = re.compile(
    r"^\s*([0-9a-fA-F]+):.*\bcall\s+(?:0x)?([0-9][0-9a-fA-F]*)\b"
)


def read_analysis_names(path: Path) -> dict[int, str]:
    result: dict[int, str] = {}
    for line in path.read_text(encoding="utf-8-sig").splitlines():
        if not line or line.startswith("#"):
            continue
        columns = line.split("\t")
        result[int(columns[0], 16)] = columns[-1]
    return result


def read_phases(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def phase_for(
    call_site: int,
    phases: list[dict[str, str]],
) -> dict[str, str]:
    for phase in phases:
        if int(phase["start"], 16) <= call_site < int(phase["end"], 16):
            return phase
    raise ValueError(f"call site 0x{call_site:08X} is outside GFMain phases")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", required=True, type=Path)
    parser.add_argument(
        "--root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
    )
    parser.add_argument("--objdump", default="objdump")
    args = parser.parse_args()

    root = args.root.resolve()
    executable = args.exe.resolve()
    objdump = shutil.which(args.objdump)
    if objdump is None:
        raise SystemExit(f"objdump not found: {args.objdump}")
    if not executable.is_file():
        raise SystemExit(f"retail executable not found: {executable}")

    phases = read_phases(root / "rebuild/integration/gfmain_phases.tsv")
    start = min(int(item["start"], 16) for item in phases)
    end = max(int(item["end"], 16) for item in phases)
    names = read_analysis_names(root / "ghidra_out/engine_api.tsv")

    process = subprocess.run(
        [
            objdump,
            "-d",
            "-Mintel",
            f"--start-address=0x{start:X}",
            f"--stop-address=0x{end:X}",
            str(executable),
        ],
        capture_output=True,
        text=True,
        check=True,
    )

    rows: list[dict[str, str]] = []
    for line in process.stdout.splitlines():
        match = INSTRUCTION_CALL.match(line)
        if match is None:
            continue
        call_site = int(match.group(1), 16)
        target = int(match.group(2), 16)
        phase = phase_for(call_site, phases)
        rows.append(
            {
                "phase": phase["phase"],
                "call_site": f"{call_site:08x}",
                "target": f"{target:08x}",
                "analysis_name": names.get(target, ""),
            }
        )

    output = root / "rebuild/integration/gfmain_calls.tsv"
    temporary = output.with_suffix(".tsv.tmp")
    with temporary.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(
            stream,
            delimiter="\t",
            fieldnames=["phase", "call_site", "target", "analysis_name"],
            lineterminator="\n",
        )
        writer.writeheader()
        writer.writerows(rows)
    temporary.replace(output)

    print(
        f"WROTE {output} direct_calls={len(rows)} "
        f"unique_targets={len({row['target'] for row in rows})}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
