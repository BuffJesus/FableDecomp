#!/usr/bin/env python3
"""Index and extract Fable II Lua scripts from gamescripts_r.bnk.

This is a small RE helper around bnk_repack.py. It intentionally avoids guessing game
semantics; it just gives repeatable access to script bytes and strings.
"""
import argparse
import os
import re
import subprocess
import sys
from pathlib import Path

import bnk_repack


DEFAULT_BNK = Path("Fable2Recomp/assets/game/data/gamescripts_r.bnk")


def entry_bytes(entry):
    raw = entry["raw"]
    if len(raw) > 32768:
        out = bytearray()
        for pos in range(0, len(raw), 32768):
            block = raw[pos : pos + 32768]
            try:
                out.extend(bnk_repack.zlib.decompressobj(15).decompress(block))
            except bnk_repack.zlib.error as exc:
                raise RuntimeError(
                    f"failed to decompress {entry['name']} block at raw offset {pos}: {exc}"
                ) from exc
        return bytes(out[: entry["dsz"]])

    out = bytearray()
    pos = 0
    targets = entry.get("chunks") or [entry["dsz"]]
    for target in targets:
        dco = bnk_repack.zlib.decompressobj(15)
        try:
            chunk = dco.decompress(raw[pos:], target)
        except bnk_repack.zlib.error as exc:
            raise RuntimeError(f"failed to decompress {entry['name']} at raw offset {pos}: {exc}") from exc
        out.extend(chunk)
        tail = dco.unconsumed_tail or dco.unused_data
        consumed = len(raw[pos:]) - len(tail)
        if consumed <= 0:
            break
        pos += consumed
        # Some BNK entries are a sequence of independent zlib chunks; others are effectively
        # one stream. The table records the decompressed chunk sizes, so stop after all targets.
    return bytes(out[: entry["dsz"]])


def is_script(entry):
    return entry["name"].lower().endswith(".lua")


def printable_strings(data, min_len=4):
    out = []
    cur = bytearray()
    for b in data:
        if 32 <= b <= 126:
            cur.append(b)
        else:
            if len(cur) >= min_len:
                out.append(cur.decode("ascii", "replace"))
            cur.clear()
    if len(cur) >= min_len:
        out.append(cur.decode("ascii", "replace"))
    return out


def load_entries(path):
    _base, entries = bnk_repack.read_bnk(str(path))
    return [e for e in entries if is_script(e)]


def command_list(args):
    scripts = load_entries(args.bnk)
    for e in sorted(scripts, key=lambda x: x["name"].lower()):
        print(f"{e['dsz']:7d}  {e['name']}")
    print(f"\n{len(scripts)} Lua scripts")


def command_search(args):
    scripts = load_entries(args.bnk)
    patterns = [re.compile(p, re.IGNORECASE if args.ignore_case else 0) for p in args.pattern]
    errors = 0
    for e in sorted(scripts, key=lambda x: x["name"].lower()):
        try:
            data = entry_bytes(e)
        except RuntimeError as exc:
            errors += 1
            if args.show_errors:
                print(f"[skip] {exc}", file=sys.stderr)
            continue
        strings = printable_strings(data, args.min_len)
        matches = []
        for s in strings:
            if all(p.search(s) for p in patterns):
                matches.append(s)
        if matches:
            print(f"{e['name']} ({e['dsz']} bytes)")
            for m in matches[: args.max_matches]:
                print(f"  {m}")
            if len(matches) > args.max_matches:
                print(f"  ... {len(matches) - args.max_matches} more")
    if errors:
        print(f"\nSkipped {errors} unreadable entries", file=sys.stderr)


def command_extract(args):
    scripts = load_entries(args.bnk)
    args.out.mkdir(parents=True, exist_ok=True)
    count = 0
    for e in scripts:
        if args.name and args.name.lower() not in e["name"].lower():
            continue
        target = args.out / e["name"].replace("\\", "/")
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(entry_bytes(e))
        count += 1
    print(f"extracted {count} scripts to {args.out}")


def command_disasm(args):
    scripts = load_entries(args.bnk)
    hit = None
    needle = args.name.lower().replace("/", "\\")
    exact = []
    partial = []
    for e in scripts:
        name = e["name"].lower()
        base = os.path.basename(name.replace("\\", os.sep))
        if needle == name or needle == base or needle == os.path.splitext(base)[0]:
            exact.append(e)
        elif needle in name:
            partial.append(e)
    if exact:
        hit = sorted(exact, key=lambda x: len(x["name"]))[0]
    elif partial:
        hit = sorted(partial, key=lambda x: len(x["name"]))[0]
    if not hit:
        raise SystemExit(f"script not found: {args.name}")
    tmp = args.out or Path("tools/lua_mod/_disasm_input.luac")
    tmp.parent.mkdir(parents=True, exist_ok=True)
    tmp.write_bytes(entry_bytes(hit))
    luadis = Path(__file__).with_name("luadis.py")
    subprocess.run([sys.executable, str(luadis), str(tmp)], check=True)


def build_parser():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--bnk", type=Path, default=DEFAULT_BNK)
    sub = p.add_subparsers(required=True)

    s = sub.add_parser("list")
    s.set_defaults(func=command_list)

    s = sub.add_parser("search")
    s.add_argument("pattern", nargs="+")
    s.add_argument("--ignore-case", action="store_true", default=True)
    s.add_argument("--min-len", type=int, default=4)
    s.add_argument("--max-matches", type=int, default=20)
    s.add_argument("--show-errors", action="store_true")
    s.set_defaults(func=command_search)

    s = sub.add_parser("extract")
    s.add_argument("--out", type=Path, default=Path("tools/lua_mod/extracted"))
    s.add_argument("--name")
    s.set_defaults(func=command_extract)

    s = sub.add_parser("disasm")
    s.add_argument("name")
    s.add_argument("--out", type=Path)
    s.set_defaults(func=command_disasm)
    return p


def main(argv=None):
    args = build_parser().parse_args(argv)
    args.func(args)


if __name__ == "__main__":
    main()
