#!/usr/bin/env python3
"""Migrate decompilation artifacts into a scalable address-sharded layout.

Raw/generated material is grouped by address for machine scalability. Curated
source and tests remain one function per translation unit (required for retail
object comparison), but live in the same two-byte address shards. Human
navigation is provided by the generated artifact index and manifest metadata.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
import re
import shutil
import subprocess
from dataclasses import asdict, dataclass
from datetime import datetime, timedelta
from pathlib import Path

from artifact_layout import address_from_filename, normalize_address, sharded_file


WAVES = ("primary", "retry", "wave2", "wave3")
DATE_DIRECTORY_RE = re.compile(r"^\d{4}-\d{2}-\d{2}$")
REPORT_FUNCTION_RE = re.compile(
    r"(?is)(?:\*\*Function:\*\*|Function:)[^\r\n]{0,300}?"
    r"(?:\bat\b|@)\s*(?:0x)?([0-9a-f]{8})\b"
)
QUEUE_RUN_RE = re.compile(
    r"\bRUN\s+target=0x(?P<address>[0-9a-f]{8})\s+slug=(?P<slug>\S+)",
    re.IGNORECASE,
)


@dataclass(frozen=True)
class Move:
    category: str
    source: str
    destination: str


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def assert_child(path: Path, parent: Path) -> None:
    try:
        path.resolve().relative_to(parent.resolve())
    except ValueError as exc:
        raise RuntimeError(f"refusing path outside {parent}: {path}") from exc


def active_decomp_processes() -> list[dict[str, object]]:
    if os.name != "nt":
        return []
    command = [
        "powershell",
        "-NoProfile",
        "-Command",
        (
            "Get-CimInstance Win32_Process | "
            "Where-Object { $_.ProcessId -ne $PID -and ("
            "$_.CommandLine -like '*run_re_agent_*queue.ps1*' -or "
            "$_.CommandLine -like '*run_rebuild_refresh.ps1*' -or "
            "$_.CommandLine -like '*re-agent*.yaml*') } | "
            "Select-Object ProcessId,Name,CommandLine | ConvertTo-Json -Compress"
        ),
    ]
    process = subprocess.run(command, capture_output=True, text=True, check=False)
    if process.returncode != 0 or not process.stdout.strip():
        return []
    data = json.loads(process.stdout)
    return data if isinstance(data, list) else [data]


def plan_address_files(
    moves: list[Move],
    category: str,
    root: Path,
    *,
    leaf: bool = False,
) -> None:
    if not root.exists():
        return
    for source in sorted(path for path in root.rglob("*") if path.is_file()):
        address = address_from_filename(source)
        if address is None:
            continue
        destination = sharded_file(root, address, source.name, leaf=leaf)
        if source.resolve() == destination.resolve():
            continue
        moves.append(Move(category, str(source), str(destination)))


def report_address(path: Path) -> str | None:
    address = address_from_filename(path)
    if address is not None:
        return address
    if path.suffix.lower() != ".json":
        return None
    try:
        text = path.read_text(encoding="utf-8-sig", errors="replace")
    except OSError:
        return None
    match = REPORT_FUNCTION_RE.search(text)
    if match is None:
        return None
    try:
        return normalize_address(match.group(1))
    except ValueError:
        return None


def load_slug_addresses(lift_root: Path) -> dict[str, str]:
    result: dict[str, str] = {}
    state_root = lift_root / "state"
    if not state_root.exists():
        return result
    for path in state_root.glob("*queue.log"):
        try:
            text = path.read_text(encoding="utf-8-sig", errors="replace")
        except OSError:
            continue
        for match in QUEUE_RUN_RE.finditer(text):
            result[match.group("slug").lower()] = match.group("address").lower()
    return result


def address_from_slug(name: str, slug_addresses: dict[str, str]) -> str | None:
    lowered = name.lower()
    matches = [
        (len(slug), address)
        for slug, address in slug_addresses.items()
        if slug in lowered
    ]
    return max(matches, default=(0, None))[1]


def plan_report_logs(moves: list[Move], lift_root: Path) -> None:
    for wave in WAVES:
        logs_root = lift_root / "reports" / wave / "logs"
        if not logs_root.exists():
            continue
        for source in sorted(path for path in logs_root.rglob("*") if path.is_file()):
            day = datetime.fromtimestamp(source.stat().st_mtime).strftime("%Y-%m-%d")
            address = report_address(source)
            if address is None:
                hour = datetime.fromtimestamp(source.stat().st_mtime).strftime("%H")
                destination = logs_root / day / "_unassigned" / hour / source.name
            else:
                destination = sharded_file(
                    logs_root / day,
                    address,
                    source.name,
                    leaf=True,
                )
            if source.resolve() == destination.resolve():
                continue
            moves.append(Move("agent-report-log", str(source), str(destination)))


def plan_transcripts(moves: list[Move], lift_root: Path) -> None:
    logs_root = lift_root / "logs"
    if not logs_root.exists():
        return
    slug_addresses = load_slug_addresses(lift_root)
    for source in sorted(logs_root.rglob("*.log")):
        if not source.is_file():
            continue
        relative_parts = source.relative_to(logs_root).parts
        day_index = next(
            (
                index
                for index, part in enumerate(relative_parts[:-1])
                if DATE_DIRECTORY_RE.match(part)
            ),
            None,
        )
        if day_index is None:
            continue
        day_root = logs_root.joinpath(*relative_parts[: day_index + 1])
        address = address_from_filename(source)
        if address is None:
            address = address_from_slug(source.name, slug_addresses)
        if address is None:
            hour = datetime.fromtimestamp(source.stat().st_mtime).strftime("%H")
            destination = day_root / "_unassigned" / hour / source.name
        else:
            destination = sharded_file(day_root, address, source.name, leaf=True)
        if source.resolve() == destination.resolve():
            continue
        moves.append(Move("agent-transcript", str(source), str(destination)))


def update_catalog_paths(path: Path) -> tuple[str, int]:
    text = path.read_text(encoding="utf-8")
    changed = 0

    def replace(match: re.Match[str]) -> str:
        nonlocal changed
        filename = match.group("path").replace("\\", "/")
        basename = Path(filename).name
        address = address_from_filename(basename)
        if address is None:
            return match.group(0)
        relative = f"{address[:2]}/{address[2:4]}/{basename}"
        if filename == relative:
            return match.group(0)
        changed += 1
        return f"{match.group('prefix')}{relative}{match.group('suffix')}"

    pattern = re.compile(
        r"(?P<prefix>\b(?:Source|TestSource)\s*=\s*')"
        r"(?P<path>[^']+)(?P<suffix>')"
    )
    return pattern.sub(replace, text), changed


def apply_moves(moves: list[Move], workspace: Path) -> dict[str, int]:
    moved = 0
    collisions = 0
    identical = 0
    for item in moves:
        source = Path(item.source)
        destination = Path(item.destination)
        assert_child(source, workspace)
        assert_child(destination, workspace)
        if source.resolve() == destination.resolve() or not source.exists():
            continue
        if destination.exists():
            collisions += 1
            if source.stat().st_size == destination.stat().st_size and sha256(source) == sha256(destination):
                identical += 1
            continue
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.move(str(source), str(destination))
        moved += 1
    return {"moved": moved, "collisions": collisions, "identical_collisions": identical}


def relative_or_blank(path: Path | None, root: Path) -> str:
    if path is None:
        return ""
    return path.resolve().relative_to(root.resolve()).as_posix()


def build_index(root: Path) -> list[dict[str, str]]:
    by_address: dict[str, dict[str, object]] = {}
    modules: dict[str, str] = {}
    compile_manifest = root / "rebuild" / "compile-gate" / "vc71-compiled.tsv"
    if compile_manifest.exists():
        with compile_manifest.open(encoding="utf-8-sig", newline="") as stream:
            for item in csv.DictReader(stream, delimiter="\t"):
                try:
                    address = normalize_address(item.get("address", ""))
                except ValueError:
                    continue
                modules[address] = item.get("module", "")

    def row(address: str) -> dict[str, object]:
        return by_address.setdefault(
            address,
            {
                "address": address,
                "module": modules.get(address, ""),
                "agent_sources": [],
                "compiled_source": "",
                "test_source": "",
                "snapshot": "",
                "build_directory": "",
            },
        )

    for wave in WAVES:
        code_root = root / "lift" / "reports" / wave / "code"
        if not code_root.exists():
            continue
        for source in code_root.rglob("*.cpp"):
            address = address_from_filename(source)
            if address is not None:
                row(address)["agent_sources"].append(relative_or_blank(source, root))

    for key, directory in (
        ("compiled_source", root / "rebuild" / "src" / "compiled"),
        ("test_source", root / "rebuild" / "tests"),
        ("snapshot", root / "rebuild" / "candidates" / "snapshots"),
    ):
        if not directory.exists():
            continue
        for source in directory.rglob("*.cpp"):
            address = address_from_filename(source)
            if address is not None:
                row(address)[key] = relative_or_blank(source, root)

    build_root = root / "rebuild" / "build" / "candidates-Release"
    if build_root.exists():
        for directory in build_root.glob("*/*/*"):
            if directory.is_dir():
                try:
                    address = normalize_address(directory.name)
                except ValueError:
                    continue
                row(address)["build_directory"] = relative_or_blank(directory, root)

    result: list[dict[str, str]] = []
    for address in sorted(by_address):
        current = by_address[address]
        result.append(
            {
                "address": address,
                "module": str(current["module"]),
                "agent_sources": ";".join(sorted(set(current["agent_sources"]))),
                "compiled_source": str(current["compiled_source"]),
                "test_source": str(current["test_source"]),
                "snapshot": str(current["snapshot"]),
                "build_directory": str(current["build_directory"]),
            }
        )
    return result


def write_index(path: Path, rows: list[dict[str, str]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + ".tmp")
    with temporary.open("w", encoding="utf-8", newline="") as stream:
        fields = list(rows[0]) if rows else [
            "address",
            "module",
            "agent_sources",
            "compiled_source",
            "test_source",
            "snapshot",
            "build_directory",
        ]
        writer = csv.DictWriter(stream, delimiter="\t", fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    temporary.replace(path)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--apply", action="store_true")
    parser.add_argument("--allow-active", action="store_true")
    parser.add_argument("--minimum-age-minutes", type=int, default=0)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()

    root = args.root.resolve()
    lift_root = root / "lift"
    rebuild = root / "rebuild"
    processes = active_decomp_processes()
    if args.apply and processes and not args.allow_active:
        raise SystemExit(
            "refusing full layout migration while decomp/refresh processes are active: "
            + ", ".join(str(item.get("ProcessId")) for item in processes)
        )

    moves: list[Move] = []
    for wave in WAVES:
        plan_address_files(
            moves,
            "agent-source",
            lift_root / "reports" / wave / "code",
        )
    plan_report_logs(moves, lift_root)
    plan_transcripts(moves, lift_root)
    plan_address_files(
        moves,
        "agent-decompile-cache",
        lift_root / ".cache" / "re-agent-decompile",
    )
    plan_address_files(
        moves,
        "candidate-snapshot",
        rebuild / "candidates" / "snapshots",
    )
    plan_address_files(
        moves,
        "compiled-source",
        rebuild / "src" / "compiled",
    )
    plan_address_files(
        moves,
        "behavior-test",
        rebuild / "tests",
    )
    plan_address_files(
        moves,
        "candidate-build",
        rebuild / "build" / "candidates-Release",
        leaf=True,
    )
    plan_address_files(
        moves,
        "land-verification",
        rebuild / "build" / "landverify",
        leaf=True,
    )

    cutoff = datetime.now() - timedelta(minutes=max(0, args.minimum_age_minutes))
    moves = [
        item
        for item in moves
        if datetime.fromtimestamp(Path(item.source).stat().st_mtime) <= cutoff
    ]

    catalog = rebuild / "build_candidates.ps1"
    catalog_text, catalog_updates = update_catalog_paths(catalog)
    results = {"moved": 0, "collisions": 0, "identical_collisions": 0}
    if args.apply:
        results = apply_moves(moves, root)
        if catalog_updates:
            catalog.write_text(catalog_text, encoding="utf-8")
        write_index(rebuild / "ARTIFACT_INDEX.tsv", build_index(root))

    categories: dict[str, int] = {}
    for item in moves:
        categories[item.category] = categories.get(item.category, 0) + 1
    summary = {
        "mode": "apply" if args.apply else "preview",
        "planned_moves": len(moves),
        "catalog_path_updates": catalog_updates,
        "active_processes": len(processes),
        "categories": categories,
        **results,
    }
    if args.json:
        print(json.dumps(summary, indent=2))
    else:
        print(
            f"mode={summary['mode']} planned={summary['planned_moves']} "
            f"moved={summary['moved']} collisions={summary['collisions']} "
            f"catalog_updates={summary['catalog_path_updates']}"
        )
        for category, count in sorted(categories.items()):
            print(f"  {category}: {count}")
    if args.apply and results["collisions"]:
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
