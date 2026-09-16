"""Stage a NoviCompatibility Lua package that runs the New Oakvale readable plus converter-unit
readable packages (Orchard Farm) through the retail-override path, and assemble the sidecar bundle.

    python tools/script_recovery/build_unit_playtest_package.py --unit orchard_farm \
        --oakvale work/new-oakvale-original-fse-20260912/local-candidate-v4/NoviCompatibility \
        --dll work/new-oakvale-original-fse-20260912/sidecar-abi-v2/Release/FableScriptExtender.dll \
        --bundle work/new-oakvale-original-fse-20260912/local-candidate-v5

Output layout (bundle/NoviCompatibility):
    NewOakValeIntro/...        verbatim from --oakvale
    <Package>/...              verbatim from refs/script_recovery/lifted/<Unit>/readable/FSE
    quests.lua                 `Quests = {}` (nothing registered as a custom quest)
    retail_override.lua        Oakvale entry + one entry per unit script (native allocator replaced by name)
"""
from __future__ import annotations

import argparse
import json
import re
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.script_units import unit as script_unit  # noqa: E402
from tools.script_recovery.build_novi_compat_bundle import assemble, WORK  # noqa: E402

OVERRIDE_HEAD = '''-- Single-authority retail override profile: New Oakvale intro + converter units.
-- DEFAULT OFF in source control. Deployment may enable this only for the explicit
-- disposable playtest after preserving rollback copies of the installed profile.
RetailOverrides = {
    enabled = true,
    disposableSaveAcknowledgement = "I UNDERSTAND THIS OVERRIDE MAY CORRUPT DISPOSABLE SAVES",
    allowUnverifiedDisposable = true,
    entries = {
'''


def oakvale_entry(oakvale):
    text = (oakvale / 'retail_override.lua').read_text(encoding='utf-8')
    start = text.index('        {\n            nativeName = "Q_NewOakValeIntro"')
    end = text.index('        },\n', start) + len('        },\n')
    return text[start:end]


def unit_entries(unit_name, first_id):
    u = script_unit(unit_name)
    report = json.loads((ROOT / 'refs/script_recovery/lifted' / u['package'] / 'draft/CONVERSION_REPORT.json').read_text(encoding='utf-8'))
    entries, next_id = [], first_id
    for unit in report['units']:
        lines = [f'        {{\n            nativeName = "{unit["script"]}",\n            file = "{unit["package"]}/{unit["package"]}",\n'
                 '            mode = "override",\n            evidenceLevel = "converter-readable",\n'
                 '            mutatingCallsAllowed = true,\n            saveWritesAllowed = true,\n']
        bindings = unit.get('bindingFiles', {})
        if bindings:
            lines.append('            entity_scripts = {\n')
            for name, file in bindings.items():
                lines.append(f'                {{ name = "{name}", file = "{file}", id = {next_id} }},\n')
                next_id += 1
            lines.append('            },\n')
        lines.append('        },\n')
        entries.append(''.join(lines))
    return ''.join(entries), next_id


def stage(unit_name, oakvale, out):
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)
    shutil.copytree(oakvale / 'NewOakValeIntro', out / 'NewOakValeIntro')
    u = script_unit(unit_name)
    readable = ROOT / 'refs/script_recovery/lifted' / u['package'] / 'readable/FSE'
    for child in readable.iterdir():
        if child.is_dir():
            shutil.copytree(child, out / child.name)
    (out / 'quests.lua').write_text('-- Retail overrides only; nothing registered as a custom quest.\nQuests = {}\n', encoding='utf-8')
    oak = oakvale_entry(oakvale)
    ids = [int(x) for x in re.findall(r'id = (\d+)', oak)]
    units, _ = unit_entries(unit_name, max(ids) + 1 if ids else 200)
    (out / 'retail_override.lua').write_text(OVERRIDE_HEAD + oak + units + '    },\n}\n', encoding='utf-8')
    return out


def main():
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--unit', default='orchard_farm')
    a.add_argument('--oakvale', type=Path, default=WORK / 'local-candidate-v4' / 'NoviCompatibility')
    a.add_argument('--stage', type=Path, default=ROOT / 'work' / 'unit_playtest_stage')
    a.add_argument('--dll', type=Path, default=WORK / 'sidecar-abi-v2' / 'Release' / 'FableScriptExtender.dll')
    a.add_argument('--template', type=Path, default=WORK / 'local-candidate-v2')
    a.add_argument('--bundle', type=Path, default=WORK / 'local-candidate-v5')
    args = a.parse_args()
    staged = stage(args.unit, args.oakvale, args.stage)
    print('staged', staged)
    assemble(args.bundle, args.template, staged, args.dll)
    print('bundle ready:', args.bundle)


if __name__ == '__main__':
    main()
