"""Build the Arena snapshot checks and compile its binding against real x86 FSE headers.

Creates a new output directory; never modifies or builds the supplied sidecar.
The compile probe instantiates sol's actual registration code but does not link
or deploy a DLL. The executable uses synthetic definition memory.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess

from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment, find_vcvars, sha

ROOT = Path(__file__).resolve().parents[2]
BINDINGS = Path(__file__).with_name('runtime_bindings')


def run(forge, output):
    forge, output = forge.resolve(), output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    env = compiler_environment(find_vcvars())
    compiler = shutil.which('cl.exe', path=env['PATH'])
    if not compiler:
        raise RuntimeError('MSVC compiler not found')
    runtime = forge / 'FableScriptExtender'
    original = runtime / 'NoviUnitBindings.h'
    text = original.read_text(encoding='utf-8-sig')
    marker = 'inline void RegisterNoviUnitBindings('
    if text.count(marker) != 1 or 'RegisterNoviArenaRounds' in text:
        raise ValueError('Review changed sidecar registration before staging')
    text = text.replace(marker, '#include "NoviArenaRounds.h"\n\n' + marker)
    signature = 'sol::usertype<CScriptThing>& thing) {'
    if text.count(signature) != 1:
        raise ValueError('Unexpected sidecar registration signature')
    text = text.replace(signature, signature + '\n    RegisterNoviArenaRounds(quest);')
    (output / original.name).write_text(text, encoding='utf-8')
    for name in ('NoviArenaRounds.h', 'arena_round_snapshot.h'):
        shutil.copyfile(BINDINGS / name, output / name)
    probe = output / 'binding_probe.cpp'
    probe.write_text('#include "NoviUnitBindings.h"\n'
                     'void CheckArenaBinding(sol::usertype<LuaQuestState>& quest) {\n'
                     '    RegisterNoviArenaRounds(quest);\n}\n', encoding='utf-8')
    check = Path(__file__).with_name('runtime_checks') / 'arena_round_snapshot.cpp'
    common = [compiler, '/nologo', '/std:c++17', '/EHsc', '/MT', '/DWIN32', '/D_WINDOWS', '/bigobj']
    commands = [
        common + ['/W4', '/WX', str(check.resolve()), '/Fo' + str(output / 'snapshot.obj'),
                  '/Fe' + str(output / 'snapshot.exe')],
        [str(output / 'snapshot.exe')],
        common + ['/c', '/I' + str(output), '/I' + str(runtime),
                  '/I' + str(forge / 'Vendor'), '/I' + str(forge / 'Vendor/sol'),
                  '/I' + str(forge / 'Vendor/lua'), str(probe),
                  '/Fo' + str(output / 'binding_probe.obj')],
    ]
    report = {'passed': False, 'scope': __doc__, 'commands': [], 'inputs': [
        {'path': str(p), 'sha256': sha(p)} for p in [original, check, *sorted(BINDINGS.glob('*.h'))]]}
    try:
        for index, command in enumerate(commands):
            result = subprocess.run(command, cwd=output, env=env, capture_output=True, text=True)
            (output / f'check_{index}.log').write_text(result.stdout + result.stderr, encoding='utf-8')
            report['commands'].append({'argv': command, 'exitCode': result.returncode})
            result.check_returncode()
        report['passed'] = True
    finally:
        (output / 'report.json').write_text(json.dumps(report, indent=2), encoding='utf-8')
    print(json.dumps({'passed': True, 'output': str(output)}))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--forge-root', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    run(args.forge_root, args.output)
