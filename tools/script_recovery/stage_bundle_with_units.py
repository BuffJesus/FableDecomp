"""Derive a new local playtest bundle from an existing one: same Lua, a replacement NoviCompatibility.dll and extra
converter units added as retail overrides. Everything already in the base bundle is copied byte for byte (runs/ and
logs excluded), so a run on the new bundle differs from the base only by what is named here.

    python tools/script_recovery/stage_bundle_with_units.py --base v16 --out v17 \
        --dll <scratch>/sidecar_v17/Release/FableScriptExtender.dll --unit bandit_camp \
        --note "v16 + MsgGetThingsKilledGroups sidecar binding + Bandit Camp converter units"

The manifest is recomputed over every staged file (the launcher's preflight checks each hash).
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.build_novi_compat_bundle import WORK  # noqa: E402
from tools.script_recovery.build_unit_playtest_package import check_override, readable_dir, unit_entries  # noqa: E402
from tools.script_recovery.script_units import unit as script_unit  # noqa: E402

SKIP = {'FableScriptExtender.log'}


def bundle_path(name):
    p = Path(name)
    return p if p.is_absolute() or p.exists() else WORK / f'local-candidate-{name}'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--base', required=True)
    a.add_argument('--out', required=True)
    a.add_argument('--dll', type=Path, help='replacement NoviCompatibility.dll')
    a.add_argument('--unit', nargs='*', default=[])
    a.add_argument('--refresh', nargs='*', default=[],
                   help='units already in the base: replace their package folders with the regenerated readable ones '
                        '(their retail_override entries stay, the script names are the same)')
    a.add_argument('--note', default='')
    args = a.parse_args()
    base, out = bundle_path(args.base), bundle_path(args.out)
    if out.exists():
        raise SystemExit(f'{out} exists; refusing to overwrite a bundle')
    shutil.copytree(base, out, ignore=lambda d, names: [n for n in names if n in SKIP or (Path(d) == base and n == 'runs')])
    if args.dll:
        shutil.copy2(args.dll, out / 'NoviCompatibility.dll')
    novi = out / 'NoviCompatibility'
    override = novi / 'retail_override.lua'
    raw = override.read_bytes().decode('utf-8')
    crlf = '\r\n' in raw
    text = raw.replace('\r\n', '\n')
    for unit_name in args.unit:
        package = script_unit(unit_name)['package']
        for child in readable_dir(package).iterdir():
            if child.is_dir():
                if (novi / child.name).exists():
                    raise SystemExit(f'{unit_name}: {child.name} already in the base bundle')
                shutil.copytree(child, novi / child.name)
        ids = [int(x) for x in re.findall(r'\bid = (\d+)', text)]
        entries, _ = unit_entries(unit_name, max(ids) + 1 if ids else 200)
        close = text.rfind('\n    },\n}')        # the `entries = { ... }` close, before the table's `}`
        if close < 0:
            raise SystemExit('retail_override.lua: unexpected tail')
        text = text[:close + 1] + entries + text[close + 1:]
    for unit_name in args.refresh:
        package = script_unit(unit_name)['package']
        for child in readable_dir(package).iterdir():
            if child.is_dir():
                if not (novi / child.name).exists():
                    raise SystemExit(f'{unit_name}: {child.name} is not in the base bundle (use --unit)')
                shutil.rmtree(novi / child.name)
                shutil.copytree(child, novi / child.name)
    override.write_bytes((text.replace('\n', '\r\n') if crlf else text).encode('utf-8'))
    names = check_override(override)
    manifest = json.loads((base / 'manifest.json').read_text(encoding='utf-8'))
    manifest['files'] = {p.relative_to(out).as_posix(): sha(p) for p in sorted(out.rglob('*'))
                         if p.is_file() and p.name not in SKIP and p.name != 'manifest.json'}
    manifest['derived_from'] = base.name
    manifest['note_' + out.name.split('-')[-1]] = args.note
    (out / 'manifest.json').write_text(json.dumps(manifest, indent=1), encoding='utf-8')
    print('bundle ready:', out)
    print('overrides:', ', '.join(names))


if __name__ == '__main__':
    main()
