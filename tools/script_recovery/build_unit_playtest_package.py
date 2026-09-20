"""Stage a NoviCompatibility Lua package that runs the New Oakvale readable plus converter-unit
readable packages through the retail-override path, and assemble the sidecar bundle.

    python tools/script_recovery/build_unit_playtest_package.py --unit orchard_farm \
        --oakvale work/new-oakvale-original-fse-20260912/local-candidate-v4/NoviCompatibility \
        --dll work/new-oakvale-original-fse-20260912/sidecar-abi-v2/Release/FableScriptExtender.dll \
        --bundle work/new-oakvale-original-fse-20260912/local-candidate-v5
    # every unit, with Aeon's LUAGameflow standing in for the retail Gameflow script (v6):
    python tools/script_recovery/build_unit_playtest_package.py --unit orchard_farm guild_training trader_conflict \
        --gameflow work/aeon_lua_ports/Gameflow/FSE/LUAGameflow/LUAGameflow.lua ... --bundle .../local-candidate-v6

Output layout (bundle/NoviCompatibility):
    NewOakValeIntro/...        verbatim from --oakvale
    <Package>/...              verbatim from refs/script_recovery/lifted/<Unit>/readable_converter/FSE (else readable/FSE)
    LUAGameflow/LUAGameflow.lua  --gameflow, registered as the override of the retail `Gameflow` script
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
    # the entry's OWN closer at 8 spaces: `'        },\n'` alone also matches inside the 12-space entity_scripts
    # closer, which cut the entry short and made the sidecar reject the whole override table (v5 shipped that way)
    end = text.index('\n        },\n', start) + len('\n        },\n')
    return text[start:end]


def check_override(path):
    """The sidecar rejects the whole table on a Lua error, silently falling back to retail: parse it here."""
    from lupa.lua54 import LuaRuntime
    lua = LuaRuntime()
    lua.execute(path.read_text(encoding='utf-8'))
    entries = lua.globals().RetailOverrides.entries
    names = [entries[i].nativeName for i in range(1, len(entries) + 1)]
    if len(set(names)) != len(names):
        raise SystemExit(f'retail_override.lua: duplicate nativeName in {names}')
    return names


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


def readable_dir(package):
    """The generated readable stage: `readable_converter/` where the old hand-reviewed `readable/` was kept
    (GuildTraining), else `readable/`; either must carry a READABLE_REPORT.json (a generated stage)."""
    for name in ('readable_converter', 'readable'):
        d = ROOT / 'refs/script_recovery/lifted' / package / name
        if (d / 'READABLE_REPORT.json').exists():
            return d / 'FSE'
    raise SystemExit(f'{package}: no generated readable stage (READABLE_REPORT.json) under readable_converter/ or readable/')


def gameflow_entry(lua):
    """Aeon's LUAGameflow port stands in for the retail master script: the same identity-preserving override
    as the units (`nativeName = "Gameflow"`, allocator replaced by name), so PostSavePosition persists under
    the retail quest and its `MsgOnQuestCompleted` polling of our units keeps the event semantics."""
    return ('        {\n            nativeName = "Gameflow",\n'
            f'            file = "{lua.parent.name}/{lua.stem}",\n'
            '            mode = "override",\n            evidenceLevel = "aeon-port",\n'
            '            mutatingCallsAllowed = true,\n            saveWritesAllowed = true,\n        },\n')


def stage(unit_names, oakvale, out, gameflow=None):
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)
    shutil.copytree(oakvale / 'NewOakValeIntro', out / 'NewOakValeIntro')
    for unit_name in unit_names:
        u = script_unit(unit_name)
        for child in readable_dir(u['package']).iterdir():
            if child.is_dir():
                if (out / child.name).exists():
                    raise SystemExit(f'{unit_name}: package folder {child.name} already staged by another unit')
                shutil.copytree(child, out / child.name)
    if gameflow:
        (out / gameflow.parent.name).mkdir()
        shutil.copy2(gameflow, out / gameflow.parent.name / gameflow.name)
    (out / 'quests.lua').write_text('-- Retail overrides only; nothing registered as a custom quest.\nQuests = {}\n', encoding='utf-8')
    oak = oakvale_entry(oakvale)
    ids = [int(x) for x in re.findall(r'id = (\d+)', oak)]
    next_id, units = (max(ids) + 1 if ids else 200), ''
    for unit_name in unit_names:
        text, next_id = unit_entries(unit_name, next_id)
        units += text
    if gameflow:
        units += gameflow_entry(gameflow)
    (out / 'retail_override.lua').write_text(OVERRIDE_HEAD + oak + units + '    },\n}\n', encoding='utf-8')
    print('retail_override.lua parses; overrides:', ', '.join(check_override(out / 'retail_override.lua')))
    return out


def main():
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--unit', nargs='+', default=['orchard_farm'])
    a.add_argument('--gameflow', type=Path, help="a LUAGameflow.lua to run as the override of the retail Gameflow script (Aeon's port)")
    a.add_argument('--oakvale', type=Path, default=ROOT / 'work' / 'oakvale_readable_stage_20260916b')   # readable + v4 retail_override.lua (Bully fix)
    a.add_argument('--stage', type=Path, default=ROOT / 'work' / 'unit_playtest_stage')
    a.add_argument('--dll', type=Path, default=WORK / 'sidecar-abi-v2' / 'Release' / 'FableScriptExtender.dll')
    a.add_argument('--template', type=Path, default=WORK / 'local-candidate-v2')
    a.add_argument('--bundle', type=Path, default=WORK / 'local-candidate-v5')
    args = a.parse_args()
    staged = stage(args.unit, args.oakvale, args.stage, args.gameflow)
    print('staged', staged)
    assemble(args.bundle, args.template, staged, args.dll)
    print('bundle ready:', args.bundle)


if __name__ == '__main__':
    main()
