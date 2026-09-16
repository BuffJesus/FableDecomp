"""Compile the proposed resource bridge with actual MSVC x86, sol and vendor Lua."""
import argparse
import hashlib
import json
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment, find_vcvars


def run(runtime, proposal, output):
    runtime, proposal, output = map(lambda p: Path(p).resolve(), (runtime, proposal, output))
    output.mkdir(parents=True, exist_ok=False)
    report = {'status': 'running', 'commands': [], 'scope': 'actual x86 bridge; engine entry points are doubles'}
    def execute(name, command, env):
        result = subprocess.run(command, cwd=output, env=env, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, text=True, errors='replace')
        (output / (name + '.log')).write_text(result.stdout, encoding='utf-8')
        report['commands'].append({'name': name, 'argv': command, 'exitCode': result.returncode})
        if result.returncode:
            raise RuntimeError(name + ' failed; see ' + str(output / (name + '.log')))
        return result.stdout
    try:
        metadata = json.loads((proposal / 'proposal.json').read_text())
        digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
        header = proposal / 'LuaRetailResources.h'
        if digest(header) != metadata['candidateSha256']:
            raise ValueError('candidate header changed since preparation')
        original = runtime / 'FableScriptExtender/LuaRetailResources.h'
        if digest(original) != metadata['sourceSha256']:
            raise ValueError('runtime bridge changed since preparation')
        vendor = runtime / 'Vendor'
        lua = vendor / 'lua'
        sources = sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c', 'luac.c', 'onelua.c'))
        harness = Path(__file__).with_name('runtime_checks') / 'resource_candidate_smoke.cpp'
        inputs = [header, original, harness, harness.with_name('retail_resources_smoke.cpp'),
                  *sources, *(runtime / 'FableScriptExtender').glob('*.h'),
                  *vendor.rglob('*.h'), *vendor.rglob('*.hpp')]
        hashes = {str(p): digest(p) for p in inputs}
        report['inputs'] = hashes
        env = compiler_environment(find_vcvars())
        compiler = shutil.which('cl.exe', path=env['PATH'])
        if not compiler or not sources:
            raise ValueError('MSVC x86 compiler or vendor Lua sources missing')
        execute('lua-build', [compiler, '/nologo', '/Bv', '/c', '/TC', '/MD', '/O2',
                             '/I' + str(lua), *map(str, sources)], env)
        objects = [str(output / (p.stem + '.obj')) for p in sources]
        execute('resource-build', [compiler, '/nologo', '/Bv', '/EHsc', '/std:c++17', '/MD', '/O2',
                                  '/I' + str(proposal), '/I' + str(runtime / 'FableScriptExtender'),
                                  '/I' + str(vendor), '/I' + str(lua), str(harness), *objects,
                                  '/Fe:resource-candidate-smoke.exe'], env)
        result = execute('resource-smoke', [str(output / 'resource-candidate-smoke.exe')], env)
        if not result.startswith('PASS:'):
            raise ValueError('resource smoke did not report PASS')
        if any(digest(Path(path)) != expected for path, expected in hashes.items()):
            raise ValueError('input changed during checks')
        report.update(status='passed', result=result.strip(),
                      binarySha256=digest(output / 'resource-candidate-smoke.exe'))
    except Exception as error:
        report.update(status='failed', error=str(error))
    (output / 'result.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, default=Path('D:/Code/ForgeFSE-retail-shadow'))
    parser.add_argument('--proposal', type=Path, default=Path('work/man_resource_integration'))
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    result = run(args.runtime, args.proposal, args.out)
    print(json.dumps({k: v for k, v in result.items() if k not in ('inputs', 'commands')}, indent=2))
    raise SystemExit(0 if result['status'] == 'passed' else 1)
