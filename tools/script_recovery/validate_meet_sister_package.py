"""Validate the disabled native-override package against actual runtime bindings.

The older reconstructed-package checker requires a legacy quests.lua identity
and assumes every non-Quest receiver is an entity. Neither fits this package.
This check does not claim metadata synchronization or native playback parity.
"""
import argparse
import json
import re
from pathlib import Path
from lupa import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]


def validate(fse, forge):
    errors = []
    lua = LuaRuntime(unpack_returned_tuples=True)
    compile_lua = lua.eval('function(s,n) local f,e=load(s,n); return f~=nil,e end')
    files = list(fse.rglob('*.lua'))
    for path in files:
        ok, error = compile_lua(path.read_text(encoding='utf-8-sig'), str(path))
        if not ok: errors.append(f'{path.name}: {error}')
    if errors: return {'errors': errors, 'lua_files': len(files)}
    lua.execute((fse / 'quests.lua').read_text())
    if list(lua.globals().Quests.items()): errors.append('Legacy quest registration must be empty')
    lua.execute((fse / 'retail_override.lua').read_text())
    config = lua.globals().RetailOverrides
    if config.enabled or config.allowUnverifiedDisposable or config.disposableSaveAcknowledgement != '':
        errors.append('Offline override must remain disabled and unacknowledged')
    entries = list(config.entries.values())
    if len(entries) != 1 or entries[0].nativeName != 'QS_MeetSister':
        errors.append('Expected exactly the QS_MeetSister native identity')
    else:
        entry = entries[0]
        entities = list(entry.entity_scripts.values())
        names = [e.name for e in entities]
        if names != ['MeetSisterMessenger', 'MeetSisterSister']: errors.append('Entity registration order/names differ')
        ids = [e.id for e in entities]
        if len(set(ids)) != 2 or not all(0 <= i < 300 for i in ids): errors.append('Invalid entity allocator IDs')
        for relative in [entry.file] + [e.file for e in entities]:
            if not (fse / (relative + '.lua')).is_file(): errors.append(f'Missing registered module: {relative}')
    reference = ROOT / 'refs/script_recovery/meet_sister/reference'
    for name in ['quest.lua', 'runtime_adapter.lua']:
        if (fse / 'MeetSister' / name).read_bytes() != (reference / name).read_bytes():
            errors.append(f'Staged {name} differs from its reviewed reference')
    adapter = (reference / 'runtime_adapter.lua').read_text()
    manager = (forge / 'FableScriptExtender/LuaManager.cpp').read_text(encoding='utf-8-sig')
    resource = (forge / 'FableScriptExtender/LuaRetailResources.h').read_text(encoding='utf-8-sig')
    calls = {'Quest': set(re.findall(r'\bquest:([A-Z]\w*)\(', adapter)),
             'RetailResources': set(re.findall(r'\bresources:([A-Z]\w*)\(', adapter))}
    registered = {'Quest': set(re.findall(r'questState_type\["(\w+)"\]', manager)),
                  'RetailResources': set(re.findall(r'type\["(\w+)"\]', resource))}
    for scope in calls:
        for name in sorted(calls[scope] - registered[scope]): errors.append(f'Missing runtime binding: {scope}.{name}')
    if 'RegisterRetailResources(lua);' not in manager: errors.append('Resource usertype registration missing')
    return {'lua_files': len(files), 'legacy_registration': 'empty', 'native_override': 'disabled',
            'runtime_api_calls': {s: sorted(n) for s,n in calls.items()}, 'errors': errors,
            'scope': 'Structure and runtime binding presence; metadata and live playback unverified'}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--fse-root', type=Path, default=ROOT / 'refs/script_recovery/reconstructed/MeetSister/FSE')
    parser.add_argument('--forge-root', type=Path, default=Path('D:/Code/ForgeFSE-retail-shadow'))
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    report = validate(args.fse_root, args.forge_root)
    text = json.dumps(report, indent=2) + '\n'
    print(text, end='')
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text, encoding='utf-8')
    return bool(report['errors'])


if __name__ == '__main__':
    raise SystemExit(main())
