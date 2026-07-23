#!/usr/bin/env python3
"""Resolve low-quality retail labels against normalized donor signatures.

This tool never writes to Ghidra. It creates a reviewable proposal ledger and
requires unique donor evidence before proposing an original symbol name.
"""
from __future__ import annotations

import argparse
import csv
import re
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path


ADDRESS_SUFFIX_RE = re.compile(r"_[0-9a-fA-F]{8}$")
CONFIDENCE_RE = re.compile(r"\[(high|medium|low)\]")


@dataclass(frozen=True)
class Label:
    address: str
    name: str
    comment: str
    confidence: str
    address_suffixed: bool


@dataclass(frozen=True)
class Signature:
    address: str
    name: str
    default_name: bool
    body_size: int
    instruction_count: int
    strict: str
    loose: str


def normalize_address(value: str) -> str:
    return value.lower().removeprefix("0x").zfill(8)


def read_labels(path: Path) -> list[Label]:
    labels: list[Label] = []
    with path.open("r", encoding="utf-8-sig", errors="replace") as stream:
        for line in stream:
            parts = line.rstrip("\r\n").split("\t", 2)
            if len(parts) < 3:
                continue
            match = CONFIDENCE_RE.search(parts[2])
            confidence = match.group(1) if match else "unknown"
            labels.append(
                Label(
                    address=normalize_address(parts[0]),
                    name=parts[1],
                    comment=parts[2],
                    confidence=confidence,
                    address_suffixed=bool(ADDRESS_SUFFIX_RE.search(parts[1])),
                )
            )
    return labels


def read_signatures(path: Path) -> list[Signature]:
    rows: list[Signature] = []
    with path.open("r", encoding="utf-8-sig", errors="replace", newline="") as stream:
        reader = csv.reader(stream, delimiter="\t")
        for parts in reader:
            if not parts or parts[0].startswith("#") or len(parts) < 7:
                continue
            rows.append(
                Signature(
                    address=normalize_address(parts[0]),
                    name=parts[1],
                    default_name=parts[2] == "1",
                    body_size=int(parts[3]),
                    instruction_count=int(parts[4]),
                    strict=parts[5],
                    loose=parts[6],
                )
            )
    return rows


def semantic_category(label: Label) -> str:
    text = f"{label.name} {label.comment}".lower()
    categories = (
        ("invalid-boundary", ("invaliddecode", "misdisassembled", "garbage", "halt_baddata")),
        ("initializer-destructor", ("initializer", "globaldtor", "dynamicinit", "atexit", "ctor", "dtor")),
        ("container-template", ("rbtree", "iterator", "vector", "allocator", "list_", "map_")),
        ("thunk-stub-wrapper", ("thunk", "stub", "wrapper", "returnvoid", "returnzero", "identity")),
        ("allocator-wrapper", ("allocstore", "allocandstore", "malloc")),
        ("field-accessor", ("field", "getter", "setter", "get", "set")),
    )
    for category, needles in categories:
        if any(needle in text for needle in needles):
            return category
    return "semantic-logic"


def donor_index(rows: list[Signature], attribute: str) -> dict[str, list[Signature]]:
    result: dict[str, list[Signature]] = defaultdict(list)
    for row in rows:
        if not row.default_name and row.name:
            result[getattr(row, attribute)].append(row)
    return result


def unique_name(matches: list[Signature]) -> str | None:
    names = {match.name for match in matches}
    return next(iter(names)) if len(names) == 1 else None


def write_manifest(labels: list[Label], out_dir: Path) -> list[Label]:
    stragglers = [label for label in labels if label.confidence == "low" or label.address_suffixed]
    out_dir.mkdir(parents=True, exist_ok=True)
    manifest = out_dir / "manifest.tsv"
    with manifest.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("address", "current_name", "confidence", "address_suffixed", "category", "comment"))
        for label in stragglers:
            writer.writerow(
                (
                    label.address,
                    label.name,
                    label.confidence,
                    int(label.address_suffixed),
                    semantic_category(label),
                    label.comment,
                )
            )
    return stragglers


def resolve(
    stragglers: list[Label],
    target_rows: list[Signature],
    donors: list[tuple[str, list[Signature]]],
    out_dir: Path,
) -> tuple[int, int]:
    target_by_address = {row.address: row for row in target_rows}
    strict_indices = [(name, donor_index(rows, "strict")) for name, rows in donors]
    loose_indices = [(name, donor_index(rows, "loose")) for name, rows in donors]
    proposals: list[tuple[str, ...]] = []
    unresolved: list[tuple[str, ...]] = []

    for label in stragglers:
        target = target_by_address.get(label.address)
        if target is None:
            unresolved.append((label.address, label.name, "missing-target-signature", semantic_category(label)))
            continue

        strict_evidence: list[tuple[str, str]] = []
        for donor_name, index in strict_indices:
            matches = index.get(target.strict, [])
            candidate = unique_name(matches)
            if candidate is not None:
                strict_evidence.append((donor_name, candidate))

        strict_names = {candidate for _, candidate in strict_evidence}
        if len(strict_names) == 1:
            proposed = next(iter(strict_names))
            donor_list = ",".join(name for name, _ in strict_evidence)
            confidence = "high" if len(strict_evidence) >= 2 else "medium"
            proposals.append(
                (
                    label.address,
                    label.name,
                    proposed,
                    confidence,
                    "strict-instruction-signature",
                    donor_list,
                    str(target.body_size),
                    str(target.instruction_count),
                )
            )
            continue

        loose_evidence: list[tuple[str, str]] = []
        for donor_name, index in loose_indices:
            matches = [
                match
                for match in index.get(target.loose, [])
                if match.instruction_count == target.instruction_count
            ]
            candidate = unique_name(matches)
            if candidate is not None:
                loose_evidence.append((donor_name, candidate))
        loose_names = {candidate for _, candidate in loose_evidence}
        if len(loose_evidence) >= 2 and len(loose_names) == 1:
            proposed = next(iter(loose_names))
            proposals.append(
                (
                    label.address,
                    label.name,
                    proposed,
                    "medium",
                    "cross-donor-loose-signature",
                    ",".join(name for name, _ in loose_evidence),
                    str(target.body_size),
                    str(target.instruction_count),
                )
            )
            continue

        reason = "ambiguous-or-no-donor-match"
        if len(strict_names) > 1:
            reason = "donor-name-disagreement"
        unresolved.append((label.address, label.name, reason, semantic_category(label)))

    proposal_path = out_dir / "proposals.tsv"
    with proposal_path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(
            ("address", "current_name", "proposed_name", "confidence", "evidence", "donors", "body_size", "instruction_count")
        )
        writer.writerows(proposals)

    unresolved_path = out_dir / "unresolved.tsv"
    with unresolved_path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("address", "current_name", "reason", "category"))
        writer.writerows(unresolved)
    return len(proposals), len(unresolved)


def write_summary(out_dir: Path, stragglers: list[Label], proposals: int | None, unresolved: int | None) -> None:
    low = sum(label.confidence == "low" for label in stragglers)
    suffixed = sum(label.address_suffixed for label in stragglers)
    categories: dict[str, int] = defaultdict(int)
    for label in stragglers:
        categories[semantic_category(label)] += 1
    lines = [
        "# Naming straggler resolver",
        "",
        f"Quality-tail functions: **{len(stragglers)}** ({low} low-confidence; {suffixed} address-suffixed).",
        "",
    ]
    if proposals is None:
        lines.append("Donor signature export is pending the active auto-RE queue.")
    else:
        lines.extend(
            (
                f"Unique donor-backed proposals: **{proposals}**.",
                f"Still unresolved: **{unresolved}**.",
                "",
                "No proposals have been applied to Ghidra; review `proposals.tsv` first.",
            )
        )
    lines.extend(("", "## Current categories", ""))
    for category, count in sorted(categories.items(), key=lambda item: (-item[1], item[0])):
        lines.append(f"- {category}: {count}")
    (out_dir / "summary.md").write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--labels", type=Path, required=True)
    parser.add_argument("--out-dir", type=Path, required=True)
    parser.add_argument("--manifest-only", action="store_true")
    parser.add_argument("--target-signatures", type=Path)
    parser.add_argument("--donor", action="append", default=[], metavar="NAME=PATH")
    args = parser.parse_args()

    labels = read_labels(args.labels)
    stragglers = write_manifest(labels, args.out_dir)
    if args.manifest_only:
        write_summary(args.out_dir, stragglers, None, None)
        print(f"manifest={len(stragglers)} out={args.out_dir}")
        return 0

    if args.target_signatures is None or not args.donor:
        parser.error("resolution requires --target-signatures and at least one --donor")
    donors: list[tuple[str, list[Signature]]] = []
    for donor_arg in args.donor:
        name, separator, path = donor_arg.partition("=")
        if not separator:
            parser.error(f"invalid donor specification: {donor_arg}")
        donors.append((name, read_signatures(Path(path))))
    proposals, unresolved = resolve(
        stragglers,
        read_signatures(args.target_signatures),
        donors,
        args.out_dir,
    )
    write_summary(args.out_dir, stragglers, proposals, unresolved)
    print(f"stragglers={len(stragglers)} proposals={proposals} unresolved={unresolved}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
