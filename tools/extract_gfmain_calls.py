#!/usr/bin/env python3
"""Audit or export GFMain's direct retail calls without taking the Ghidra lock."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path

# The validated ledger tracks reconstructed callees. These four direct calls
# are CRT allocation/cookie machinery in GFMain itself, not reconstructed
# engine callees. Keep them explicit so a new omitted engine edge fails audit.
RUNTIME_CALLS = {
    0x00402652: 0x00BFE9BC,  # operator delete, installer release
    0x00402E50: 0x00BFEA1A,  # operator new, text-bank allocation
    0x00403002: 0x00BFE9BC,  # operator delete, text-bank release
    0x00403472: 0x00BFE9F9,  # __security_check_cookie
}


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
    parser.add_argument(
        "--output",
        type=Path,
        help="write a raw four-column call inventory to this path (never the curated ledger)",
    )
    parser.add_argument(
        "--check-ledger",
        action="store_true",
        help="compare all direct retail calls with the validated GFMain call ledger without writing it",
    )
    args = parser.parse_args()

    root = args.root.resolve()
    executable = args.exe.resolve()
    if not executable.is_file():
        raise SystemExit(f"retail executable not found: {executable}")

    phases = read_phases(root / "rebuild/integration/gfmain_phases.tsv")
    start = min(int(item["start"], 16) for item in phases)
    end = max(int(item["end"], 16) for item in phases)
    names = read_analysis_names(root / "ghidra_out/engine_api.tsv")

    # GNU objdump loses alignment after branch/data islands in GFMain. Decode
    # the bounded body from the PE itself; this agrees with Ghidra's 261 calls.
    import sys

    sys.path.insert(0, str(root / "tools/decomp_pipeline"))
    import pe_oracle
    from capstone import CS_ARCH_X86, CS_MODE_32, Cs

    image = executable.read_bytes()
    offset = pe_oracle.va_to_off(pe_oracle.pe_sections(image), start)
    if offset is None:
        raise SystemExit("GFMain is outside the retail PE sections")
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    rows = []
    for instruction in decoder.disasm(image[offset : offset + end - start], start):
        if instruction.mnemonic == "call" and instruction.op_str.startswith("0x"):
            site = instruction.address
            target = int(instruction.op_str, 16)
            rows.append(
                {
                    "phase": phase_for(site, phases)["phase"],
                    "call_site": f"{site:08x}",
                    "target": f"{target:08x}",
                    "analysis_name": names.get(target, ""),
                }
            )

    if args.check_ledger:
        ledger_path = root / "rebuild/integration/gfmain_calls.tsv"
        with ledger_path.open("r", encoding="utf-8-sig", newline="") as stream:
            ledger_rows = list(csv.DictReader(stream, delimiter="\t"))
        actual = {int(row["call_site"], 16): int(row["target"], 16) for row in rows}
        ledger = {int(row["call_site"], 16): int(row["target"], 16) for row in ledger_rows}
        duplicates = len(ledger) != len(ledger_rows) or len(actual) != len(rows)
        mismatches = [
            (site, target, actual.get(site))
            for site, target in ledger.items()
            if actual.get(site) != target
        ]
        missing = {site: target for site, target in actual.items() if site not in ledger}
        correct_phases = all(
            row["phase"] == phase_for(int(row["call_site"], 16), phases)["phase"]
            for row in ledger_rows
        )
        passed = not duplicates and not mismatches and missing == RUNTIME_CALLS and correct_phases
        print(
            f"{'PASS' if passed else 'FAIL'} GFMain call ledger: "
            f"{len(ledger)} validated edges, {len(missing)} runtime edges, "
            f"{len(mismatches)} mismatches"
        )
        if not passed:
            print(f"duplicates={duplicates} phases_valid={correct_phases}")
            print(f"mismatches={mismatches[:8]}")
            print(f"unlisted={missing}")
        return 0 if passed else 1

    if args.output is None:
        raise SystemExit("raw export requires --output; use --check-ledger to audit the curated ledger")
    output = args.output.resolve()
    if output == (root / "rebuild/integration/gfmain_calls.tsv").resolve():
        raise SystemExit("refusing to overwrite the curated GFMain call ledger")
    output.parent.mkdir(parents=True, exist_ok=True)
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
