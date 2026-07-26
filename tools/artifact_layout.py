#!/usr/bin/env python3
"""Shared address-sharding rules for decompilation artifacts."""
from __future__ import annotations

import re
from pathlib import Path
from typing import Iterable, Iterator


ADDRESS_RE = re.compile(r"(?i)(?:0x)?([0-9a-f]{8})")
FUNCTION_FILENAME_RE = re.compile(r"(?i)^0x([0-9a-f]{8})_")
SUFFIX_ADDRESS_RE = re.compile(r"(?i)_([0-9a-f]{8})(?:_test)?\.cpp$")


def normalize_address(value: str) -> str:
    address = value.lower().strip().removeprefix("0x")
    if not re.fullmatch(r"[0-9a-f]{8}", address):
        raise ValueError(f"invalid 32-bit address: {value!r}")
    return address


def plausible_image_address(value: str) -> bool:
    address = int(normalize_address(value), 16)
    return 0x00400000 <= address < 0x02000000


def address_from_filename(path: Path | str) -> str | None:
    name = Path(path).name
    match = FUNCTION_FILENAME_RE.match(name)
    if match:
        return match.group(1).lower()
    match = SUFFIX_ADDRESS_RE.search(name)
    if match:
        return match.group(1).lower()
    for match in ADDRESS_RE.finditer(name):
        candidate = match.group(1).lower()
        if plausible_image_address(candidate):
            return candidate
    return None


def shard_parts(address: str) -> tuple[str, str]:
    normalized = normalize_address(address)
    return normalized[:2], normalized[2:4]


def shard_directory(root: Path, address: str, *, leaf: bool = False) -> Path:
    first, second = shard_parts(address)
    directory = root / first / second
    return directory / normalize_address(address) if leaf else directory


def sharded_file(root: Path, address: str, filename: str, *, leaf: bool = False) -> Path:
    return shard_directory(root, address, leaf=leaf) / filename


def iter_address_files(roots: Iterable[Path], suffix: str = ".cpp") -> Iterator[tuple[str, Path]]:
    for root in roots:
        if not root.exists():
            continue
        for path in root.rglob(f"*{suffix}"):
            if not path.is_file():
                continue
            address = address_from_filename(path)
            if address is not None:
                yield address, path
