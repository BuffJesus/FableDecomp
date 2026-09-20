#!/usr/bin/env python3
"""scaffold_from_stage.py -- seed the Oakvale Reborn FSE tree from the v4-proven stage.

One-shot (re-runnable with --force) copy of work/oakvale_readable_stage_20260916b
into refs/script_recovery/authored/OakvaleReborn/FSE with the package renamed:

  NewOakValeIntro/NewOakValeIntro.lua         -> OakvaleReborn/OakvaleReborn.lua
  NewOakValeIntro/native_quest_helpers.lua    -> OakvaleReborn/native_quest_helpers.lua
  NewOakValeIntro/Entities/NOVI_<X>.lua       -> OakvaleReborn/Entities/OVR_<X>.lua
  NewOakValeIntro/Entities/OVI_DeadFather.lua -> OakvaleReborn/Entities/OVR_DeadFather.lua

Only the Lua FILE paths (require(), entity_scripts.file, AddEntityBinding's second
argument) change. Every "NOVI_*" / "OVI_*" string that names a retail TNG thing,
quest section or cutscene actor is left alone -- those are the stage, and the
stage stays retail. The override keeps nativeName "Q_NewOakValeIntro" so
LUAGameflow's handoff to Q_GuildTraining is untouched.

Files that already exist under the authored tree are never overwritten unless
--force is given: the authored tree is hand-edited after this seed.
"""
from __future__ import annotations

import argparse
import pathlib
import re

REPO = pathlib.Path(__file__).resolve().parents[2]
SOURCE = REPO / 'work/oakvale_readable_stage_20260916b'
DEST = REPO / 'refs/script_recovery/authored/OakvaleReborn/FSE'
OLD_PKG, NEW_PKG = 'NewOakValeIntro', 'OakvaleReborn'


def new_entity_file(stem: str) -> str:
    if stem.startswith('NOVI_'):
        return 'OVR_' + stem[len('NOVI_'):]
    if stem.startswith('OVI_'):
        return 'OVR_' + stem[len('OVI_'):]
    return stem


def rewrite(text: str) -> str:
    # package-qualified Lua paths only: "NewOakValeIntro/Entities/NOVI_X", "NewOakValeIntro.native_quest_helpers"
    def ent(m: re.Match) -> str:
        return f'"{NEW_PKG}/Entities/{new_entity_file(m.group(1))}"'
    text = re.sub(rf'"{OLD_PKG}/Entities/(\w+)"', ent, text)
    text = text.replace(f'"{OLD_PKG}/{OLD_PKG}"', f'"{NEW_PKG}/{NEW_PKG}"')
    text = text.replace(f'"{OLD_PKG}.native_quest_helpers"', f'"{NEW_PKG}.native_quest_helpers"')
    return text


def main() -> int:
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--force', action='store_true', help='overwrite files that already exist')
    args = a.parse_args()
    plan: list[tuple[pathlib.Path, pathlib.Path]] = []
    for src in sorted(SOURCE.rglob('*.lua')):
        rel = src.relative_to(SOURCE)
        parts = list(rel.parts)
        if parts[0] == OLD_PKG:
            parts[0] = NEW_PKG
            if len(parts) == 3 and parts[1] == 'Entities':
                parts[2] = new_entity_file(src.stem) + '.lua'
            elif parts[-1] == f'{OLD_PKG}.lua':
                parts[-1] = f'{NEW_PKG}.lua'
        plan.append((src, DEST.joinpath(*parts)))
    written = skipped = 0
    for src, dst in plan:
        if dst.exists() and not args.force:
            skipped += 1
            continue
        dst.parent.mkdir(parents=True, exist_ok=True)
        dst.write_text(rewrite(src.read_text(encoding='utf-8')), encoding='utf-8', newline='\n')
        written += 1
        print(f'{src.relative_to(SOURCE).as_posix():55s} -> {dst.relative_to(DEST).as_posix()}')
    print(f'{written} written, {skipped} kept (use --force to overwrite)')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
