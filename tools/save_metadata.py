#!/usr/bin/env python3
"""Enumerate Fable save rows and decode the metadata shown by the LOAD screen.

The game does not discover manual saves by scanning the directory.  It reads the
``SaveGameNames1..50`` registry from ``Profile.bin`` and then lazily decodes the
``HEADER`` section of each referenced ``FableSave!`` file.  This module keeps
those two operations together so frontend callers cannot accidentally display an
unregistered file or treat a missing/corrupt save as loadable.

The returned row action values mirror the recovered frontend contract:
``0x11`` for a valid loadable row and ``0xDC`` for a missing or invalid row.
Autosaves are included ahead of manual registry entries when their primary save
file exists.  Companion ``.hs`` files are intentionally not required for the
metadata row; they belong to the later hero-state path.
"""

from __future__ import annotations

import argparse
import json
import struct
import sys
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

try:
    from tools.save_install import Profile
    from tools import save_edit
    from tools.save_tools import savecrc
except ImportError:  # direct ``python tools/save_metadata.py`` from repo root
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
    from tools.save_install import Profile
    from tools import save_edit
    from tools.save_tools import savecrc


HEADER_SCHEMA: Tuple[Tuple[str, str], ...] = (
    ("WorldName", "ascii"),
    ("WorldFrame", "u32"),
    ("TeleportingEnabled", "bool"),
    ("SavingEnabled", "bool"),
    ("ExperienceSpendingEnabled", "bool"),
    ("CreatureGenerationEnabled", "bool"),
    ("CreatureGenerationDisabledGroups", "u32"),
    ("HeroSleepingEnabled", "bool"),
    ("MapTableShowQuestCardsOnUsed", "bool"),
    ("MiniMapEnabled", "bool"),
    ("MiniMapActiveBeforeDisabled", "bool"),
    ("GuildMasterMessagesEnabled", "bool"),
    ("SummonerDeathExplosionAffectsHero", "bool"),
    ("MostRecentSaveType", "u32"),
    ("MostRecentSaveTypeBeforeManualSave", "u32"),
    ("MostRecentManualSaveName", "wide"),
    ("SaveGameMarkerPos", "float3"),
    ("SaveGameMarkerAngleXY", "float"),
    ("GuildSealRecallPos", "float3"),
    ("GuildSealRecallAngleXY", "float"),
    ("CurrentRegionName", "ascii"),
    ("CurrentRegionMinimapGraphicName", "ascii"),
    ("TotalTimePlayed", "float"),
)


@dataclass
class SaveRow:
    """One row in the recovered saved-games browser contract."""

    filename: str
    kind: str
    slot_index: Optional[int]
    path: str
    exists: bool
    valid: bool
    action: int
    error: str = ""
    metadata: Optional[Dict[str, Any]] = None

    def to_dict(self) -> Dict[str, Any]:
        return asdict(self)


@dataclass
class SaveBrowserData:
    profile_dir: str
    profile_crc_valid: bool
    rows: List[SaveRow]

    def to_dict(self) -> Dict[str, Any]:
        return {
            "profile_dir": self.profile_dir,
            "profile_crc_valid": self.profile_crc_valid,
            "rows": [row.to_dict() for row in self.rows],
        }


def _read_ascii(data: bytes, offset: int) -> Tuple[str, int]:
    end = data.find(b"\0", offset)
    if end < 0:
        raise ValueError("unterminated ASCII HEADER string")
    return data[offset:end].decode("latin1"), end + 1


def _read_wide(data: bytes, offset: int) -> Tuple[str, int]:
    chars: List[str] = []
    while offset + 2 <= len(data):
        value = struct.unpack_from("<H", data, offset)[0]
        offset += 2
        if value == 0:
            return "".join(chars), offset
        chars.append(chr(value))
    raise ValueError("unterminated UTF-16 HEADER string")


def decode_header(data: bytes) -> Dict[str, Any]:
    """Decode the confirmed inflated ``HEADER`` section from a save file.

    The function validates every field tag against the game's seed-0 CRC and
    rejects trailing bytes.  This prevents a plausible-looking row from being
    produced when the save uses a different section layout.
    """

    nul = data.find(b"\0")
    if nul < 0 or data[:nul] != b"HEADER":
        raise ValueError("save chunk does not begin with HEADER")
    if nul + 5 > len(data):
        raise ValueError("truncated HEADER length")
    section_length = struct.unpack_from("<I", data, nul + 1)[0]
    offset = nul + 5
    end = offset + section_length
    if end > len(data):
        raise ValueError("HEADER section length exceeds inflated chunk")

    values: Dict[str, Any] = {}
    for name, kind in HEADER_SCHEMA:
        if offset + 4 > end:
            raise ValueError("HEADER ends before field %s" % name)
        actual_tag = struct.unpack_from("<I", data, offset)[0]
        expected_tag = savecrc.getcrc(name, 0)
        offset += 4
        if actual_tag != expected_tag:
            raise ValueError(
                "HEADER tag mismatch for %s: %08x != %08x"
                % (name, actual_tag, expected_tag)
            )
        if kind == "ascii":
            value, offset = _read_ascii(data, offset)
        elif kind == "wide":
            value, offset = _read_wide(data, offset)
        elif kind == "u32":
            if offset + 4 > end:
                raise ValueError("HEADER truncates field %s" % name)
            value = struct.unpack_from("<I", data, offset)[0]
            offset += 4
        elif kind == "bool":
            if offset >= end:
                raise ValueError("HEADER truncates field %s" % name)
            value = bool(data[offset])
            offset += 1
        elif kind == "float":
            if offset + 4 > end:
                raise ValueError("HEADER truncates field %s" % name)
            value = struct.unpack_from("<f", data, offset)[0]
            offset += 4
        elif kind == "float3":
            if offset + 12 > end:
                raise ValueError("HEADER truncates field %s" % name)
            value = list(struct.unpack_from("<3f", data, offset))
            offset += 12
        else:  # pragma: no cover - schema is module-owned and exhaustive
            raise AssertionError("unknown HEADER kind %s" % kind)
        values[name] = value

    if offset != end:
        raise ValueError("HEADER has %d trailing bytes" % (end - offset))
    return values


def decode_save(path: Path) -> Dict[str, Any]:
    """Validate and decode one ``FableSave!`` file."""

    raw = path.read_bytes()
    parsed = save_edit.read_save(raw)
    return decode_header(parsed.chunk0)


def _row(profile_dir: Path, filename: str, kind: str,
         slot_index: Optional[int]) -> SaveRow:
    path = profile_dir / filename
    if not path.is_file():
        return SaveRow(
            filename, kind, slot_index, str(path), False, False, 0xDC,
            "missing save file", None)
    try:
        metadata = decode_save(path)
    except Exception as exc:  # preserve the row so the UI can mark it invalid
        return SaveRow(
            filename, kind, slot_index, str(path), True, False, 0xDC,
            "%s: %s" % (type(exc).__name__, exc), None)
    return SaveRow(filename, kind, slot_index, str(path), True, True, 0x11, "", metadata)


def enumerate_profile(profile_dir: Path | str) -> SaveBrowserData:
    """Return autosave-first rows from a profile directory.

    Manual rows come exclusively from ``Profile.bin``.  Directory files that
    have no registry entry are deliberately ignored, matching retail behavior.
    """

    profile_dir = Path(profile_dir)
    profile_path = profile_dir / "Profile.bin"
    profile, _ = _load_profile(profile_dir)
    rows: List[SaveRow] = []

    # Both names occur in the recovered PC save layouts.  Only add a reserved
    # row when the primary file exists; a missing autosave is not a visible row.
    seen = set()
    for filename in ("AutoSave", "AutoSave.qs"):
        if filename not in seen and (profile_dir / filename).is_file():
            rows.append(_row(profile_dir, filename, "autosave", None))
            seen.add(filename)

    for index, filename in sorted(profile.slots().items()):
        if filename and filename not in seen:
            rows.append(_row(profile_dir, filename, "manual", index))
            seen.add(filename)

    return SaveBrowserData(str(profile_dir), profile.verify(), rows)


def _load_profile(profile_dir: Path) -> Tuple[Profile, Path]:
    path = profile_dir / "Profile.bin"
    with path.open("rb") as stream:
        return Profile(stream.read()), path


def _main(argv: Optional[List[str]] = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("profile_dir", type=Path)
    parser.add_argument("--json", action="store_true", dest="as_json")
    args = parser.parse_args(argv)
    result = enumerate_profile(args.profile_dir)
    if args.as_json:
        print(json.dumps(result.to_dict(), indent=2, sort_keys=True))
    else:
        print("Profile: %s (CRC %s)" % (
            result.profile_dir, "OK" if result.profile_crc_valid else "INVALID"))
        for row in result.rows:
            state = "LOADABLE" if row.valid else "INVALID"
            slot = "auto" if row.slot_index is None else "slot%d" % row.slot_index
            print("  %-6s action=0x%02X %-8s %s" %
                  (slot, row.action, state, row.filename))
            if row.error:
                print("      %s" % row.error)
            elif row.metadata:
                print("      %s | %s | frame=%s | time=%s" % (
                    row.metadata["WorldName"],
                    row.metadata["CurrentRegionName"],
                    row.metadata["WorldFrame"],
                    row.metadata["TotalTimePlayed"]))
    return 0


if __name__ == "__main__":
    raise SystemExit(_main())
