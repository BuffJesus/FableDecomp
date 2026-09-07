#!/usr/bin/env python3
"""lev_theme_rebase.py - audit / repair a LEV's ground-theme palette.

A LEV carries a 256-entry theme palette at file offset 47 (LEVHeader 25 B +
LEVMapHeader 22 B), each entry `char name[128] + u32 value` (docs/formats/LEV_WRITER.md).

The u32 is the GLOBAL game.bin def entry index, not a name hash. Proven two ways:

  * retail data - Greatwood_1.lev (extracted from FinalAlbion.wad) stores
    GROUND_GRASS_NO_LOCAL_DETAIL -> 1917, and `forge defs show <root> 1917`
    is exactly `ENGINE_THEME / GROUND_GRASS_NO_LOCAL_DETAIL`.
  * FableWin symbols - CMap::AddThemeDefIndexToPalette(u32),
    CMap::GetThemePaletteIndexFromThemeDefIndex(u32),
    CMap::InitPassabilityFromThemesIfNecessary, CMap::GetThemeForMiniMapAt.
    The per-cell u8 is a palette slot; this table maps slot -> def index.

Because the index is global, ANY def added to or removed from game.bin shifts it,
and a LEV authored against a different game.bin silently points at the wrong def.
Our own authored terrain does exactly that: ForgeTest64_final.lev (and the
AshfallHollow.lev cloned from it) are a uniform 228 too low, landing on CREATURE
defs. Reads stay in bounds - CREATURE payloads are large - so it degrades quietly
into wrong passability seeding, minimap colour and camera-Z clamps rather than
crashing.

Rebasing is a pure in-place u32 rewrite: same size, every other byte preserved.

Usage:
  lev_theme_rebase.py <lev>... --game-root <install>            # audit only
  lev_theme_rebase.py <lev>... --game-root <install> --write    # repair in place
  lev_theme_rebase.py <lev>... --defs <cached defs list>        # offline audit

`--defs` takes the output of `forge defs list <game-root>`; without it the tool
runs that command itself (override the binary with --forge).
"""

from __future__ import annotations

import argparse
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

THEME_TABLE_OFFSET = 47          # LEVHeader 25 + LEVMapHeader 22
THEME_ENTRY_SIZE = 132           # char name[128] + u32 value
THEME_ENTRY_COUNT = 256
THEME_DEF_TYPE = "ENGINE_THEME"

DEFAULT_FORGE = r"D:\Code\FableForge\build\forge.exe"

# "  1917  ENGINE_THEME  210 bytes  GROUND_GRASS_NO_LOCAL_DETAIL"
DEF_ROW = re.compile(r"^\s*(\d+)\s+(\S+)\s+\d+\s+bytes\s+(\S+)\s*$")


def load_def_table(text: str):
    """Return (name -> index, index -> (type, name))."""
    by_name, by_index = {}, {}
    for line in text.splitlines():
        m = DEF_ROW.match(line)
        if not m:
            continue
        index, def_type, name = int(m.group(1)), m.group(2), m.group(3)
        by_index[index] = (def_type, name)
        # First definition of a name wins; retail names are unique.
        by_name.setdefault(name, index)
    return by_name, by_index


def read_defs(args) -> str:
    if args.defs:
        return Path(args.defs).read_text(encoding="utf-8", errors="replace")
    if not args.game_root:
        raise SystemExit("need --game-root or --defs")
    cmd = [args.forge, "defs", "list", str(args.game_root)]
    if args.bin:
        cmd.append(args.bin)
    done = subprocess.run(cmd, capture_output=True, text=True)
    if done.returncode != 0:
        raise SystemExit(f"forge defs list failed:\n{done.stdout}{done.stderr}")
    return done.stdout


def palette_entries(data: bytes):
    """Yield (slot, name, stored_value, value_offset) for named slots."""
    for slot in range(THEME_ENTRY_COUNT):
        at = THEME_TABLE_OFFSET + slot * THEME_ENTRY_SIZE
        if at + THEME_ENTRY_SIZE > len(data):
            return
        raw = data[at:at + 128]
        name = raw.split(b"\x00", 1)[0].decode("latin-1")
        if not name:
            continue
        value = struct.unpack_from("<I", data, at + 128)[0]
        yield slot, name, value, at + 128


def audit(path: Path, by_name, by_index, write: bool, backup: bool):
    data = bytearray(path.read_bytes())
    rows, fixes, unknown = [], [], []

    for slot, name, value, value_at in palette_entries(data):
        want = by_name.get(name)
        have_type = by_index.get(value, ("<out of range>", ""))[0]
        if want is None:
            unknown.append((slot, name, value))
            status = "NO SUCH DEF"
        elif want == value:
            status = "ok"
        else:
            status = f"WRONG (points at {have_type})"
            fixes.append((slot, name, value, want, value_at))
        rows.append((slot, name, value, want, status))

    print(f"\n{path}")
    print(f"  {len(rows)} named palette slots, {len(fixes)} wrong, {len(unknown)} unresolvable")
    for slot, name, value, want, status in rows:
        if status == "ok":
            continue
        print(f"    slot {slot:3d}  {name:<34} stored {value:<6} want {want}  {status}")

    if fixes and write:
        if backup:
            bak = path.with_suffix(path.suffix + ".pre-themerebase")
            if not bak.exists():
                shutil.copy2(path, bak)
                print(f"  backup -> {bak.name}")
        for _slot, _name, _value, want, value_at in fixes:
            struct.pack_into("<I", data, value_at, want)
        path.write_bytes(bytes(data))
        print(f"  rebased {len(fixes)} palette entries in place "
              f"({len(data)} bytes, size unchanged)")

    return len(fixes), len(unknown)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("levs", nargs="+", type=Path)
    ap.add_argument("--game-root")
    ap.add_argument("--defs", help="cached `forge defs list` output")
    ap.add_argument("--bin", help="def bin name, e.g. game.bin")
    ap.add_argument("--forge", default=DEFAULT_FORGE)
    ap.add_argument("--write", action="store_true", help="repair in place")
    ap.add_argument("--no-backup", action="store_true")
    args = ap.parse_args()

    by_name, by_index = load_def_table(read_defs(args))
    themes = sum(1 for t, _ in by_index.values() if t == THEME_DEF_TYPE)
    print(f"def table: {len(by_index)} entries, {themes} {THEME_DEF_TYPE}")

    wrong = unresolved = 0
    for lev in args.levs:
        w, u = audit(lev, by_name, by_index, args.write, not args.no_backup)
        wrong += w
        unresolved += u

    print(f"\ntotal: {wrong} wrong, {unresolved} unresolvable"
          f"{' (rewritten)' if args.write and wrong else ''}")
    return 1 if (wrong and not args.write) or unresolved else 0


if __name__ == "__main__":
    raise SystemExit(main())
