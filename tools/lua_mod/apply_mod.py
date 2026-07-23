#!/usr/bin/env python3
"""Apply, check, or revert the Fable II Lua hook in gamescripts_r.bnk."""
import argparse
import os
import shutil
import zlib
from pathlib import Path

import bnk_repack


DEFAULT_BNK = Path("Fable2Recomp/assets/game/data/gamescripts_r.bnk")
DEFAULT_HOOK = Path(__file__).with_name("myconsolehook0.lua")
OLD_NAME = b"miscellaneous/AppearanceEnum.lua"
NEW_NAME = b"miscellaneous/MyConsoleHook0.lua"
HOOK_ENTRY = r"scripts\miscellaneous\myconsolehook0.lua"


def script_bytes(entry):
    return zlib.decompressobj(15).decompress(entry["raw"])[: entry["dsz"]]


def find_generalsetup(entries):
    return next(e for e in entries if "generalsetupscript" in e["name"].lower())


def status(path):
    base, entries = bnk_repack.read_bnk(str(path))
    general = script_bytes(find_generalsetup(entries))
    has_hook_entry = any(e["name"].lower() == HOOK_ENTRY.lower() for e in entries)
    return {
        "base": base,
        "entries": entries,
        "general": general,
        "has_old": OLD_NAME in general,
        "has_new": NEW_NAME in general,
        "has_hook_entry": has_hook_entry,
    }


def command_check(args):
    bak = Path(str(args.bnk) + ".orig_backup")
    st = status(args.bnk)
    print(f"bnk: {args.bnk}")
    print(f"backup: {'present' if bak.exists() else 'missing'} ({bak})")
    print(f"entries: {len(st['entries'])}")
    print(f"generalsetup old slot present: {st['has_old']}")
    print(f"generalsetup hook slot present: {st['has_new']}")
    print(f"hook entry present: {st['has_hook_entry']}")


def command_revert(args):
    bak = Path(str(args.bnk) + ".orig_backup")
    if not bak.exists():
        raise SystemExit(f"backup missing: {bak}")
    shutil.copyfile(bak, args.bnk)
    print(f"reverted {args.bnk} from {bak}")


def command_apply(args):
    bak = Path(str(args.bnk) + ".orig_backup")
    if not bak.exists():
        shutil.copyfile(args.bnk, bak)
        print(f"backed up -> {bak}")
    else:
        shutil.copyfile(bak, args.bnk)
        print("restored clean from backup before applying")

    base, entries = bnk_repack.read_bnk(str(args.bnk))
    general = find_generalsetup(entries)
    data = script_bytes(general)
    if len(OLD_NAME) != len(NEW_NAME):
        raise SystemExit("internal error: replacement names differ in length")
    if data.count(OLD_NAME) != 1:
        raise SystemExit("patch target not found/unique in generalsetupscript")

    bnk_repack.replace_entry(entries, "generalsetupscript", data.replace(OLD_NAME, NEW_NAME))
    bnk_repack.add_entry(entries, HOOK_ENTRY, args.hook.read_bytes())
    bnk_repack.write_bnk(str(args.bnk), base, entries)
    print(f"applied mod: {len(bnk_repack.read_bnk(str(args.bnk))[1])} entries")


def build_parser():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("bnk", nargs="?", type=Path, default=DEFAULT_BNK)
    p.add_argument("hook", nargs="?", type=Path, default=DEFAULT_HOOK)
    p.add_argument("--check", action="store_true")
    p.add_argument("--revert", action="store_true")
    return p


def main():
    args = build_parser().parse_args()
    if args.check and args.revert:
        raise SystemExit("--check and --revert are mutually exclusive")
    if args.check:
        command_check(args)
    elif args.revert:
        command_revert(args)
    else:
        command_apply(args)


if __name__ == "__main__":
    main()
