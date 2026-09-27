"""Write `<package>/SUPERSEDED.json`: first-generation lifts (`<package>/FSE/<X>/...`) replaced by the registered
unit's readable port (`<package>/readable/FSE/<Script>/...`). audit_readability skips a listed file only while its
replacement exists. Nothing is deleted; the old lifts stay in the tree and in history.

    python -m tools.script_recovery.mark_superseded beardy_baldy dragon_boss_fight ...
    python -m tools.script_recovery.mark_superseded guardian_trophy_dealer_info --only Entities/GTDI_Maze.lua
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path

from tools.script_recovery.script_units import ROOT, unit as script_unit


def mapping(unit_name, only=None):
    spec = script_unit(unit_name)
    package = ROOT / 'refs/script_recovery/lifted' / spec['package']
    folders = [d for d in (package / 'readable/FSE').iterdir() if d.is_dir()]
    if len(folders) != 1:
        return package, {}
    new_root = folders[0]            # `V_StatueMaster`, or `DragonBossFight` for a Q_ script (the prefix is dropped)
    script = new_root.name
    old_roots = [d for d in (package / 'FSE').iterdir() if d.is_dir()]
    files = {}
    for old_root in old_roots:
        for old in sorted(old_root.rglob('*.lua')):
            rel = old.relative_to(old_root)
            if only and rel.as_posix() not in only:
                continue
            new = new_root / (f'{script}.lua' if rel.as_posix() == f'{old_root.name}.lua' else rel)
            if new.is_file():
                files[old.relative_to(package).as_posix()] = {'by': new.relative_to(package).as_posix(),
                                                              'reason': f'registered unit {unit_name} (convert_quest_unit + build_readable_unit)'}
    return package, files


def main():
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('units', nargs='+')
    a.add_argument('--only', nargs='*', help='old-root-relative files to mark (default: every file with a replacement)')
    args = a.parse_args()
    for name in args.units:
        package, files = mapping(name, set(args.only) if args.only else None)
        marker = package / 'SUPERSEDED.json'
        current = json.loads(marker.read_text(encoding='utf-8')) if marker.exists() else {'files': {}}
        current['schema'] = 'lifted-superseded/1'
        current['files'].update(files)
        marker.write_text(json.dumps(current, indent=2) + '\n', encoding='utf-8')
        print(name, len(files), 'superseded ->', marker.relative_to(ROOT))


if __name__ == '__main__':
    main()
