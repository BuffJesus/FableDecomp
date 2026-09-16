"""Compile the separate named-marker proposal, with checked real x86 API types."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.maze_native import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment, find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    _, witness = recover()
    source = Path(__file__).parent
    output = ROOT / 'work/maze_converter/runtime_proposal'
    output.mkdir(parents=True, exist_ok=True)
    for name in ('maze_marker_adapter.inc', 'maze_marker_harness.cpp', 'maze_lifecycle_harness.cpp'):
        shutil.copyfile(source / name, output / name)
    env = compiler_environment(find_vcvars())
    compiler = shutil.which('cl.exe', path=env['PATH'])
    vendor = runtime / 'Vendor'; lua = vendor / 'lua'
    sources = sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report = {'status': 'pending integration', 'runtimeChanged': False, 'nativeEvidence': witness,
        'commands': [], 'integration': [
            'Declare LuaQuestState::AddMiniMapMarkerByScriptName(const std::string&,const std::string&).',
            'Include maze_marker_adapter.inc in LuaQuestState.cpp.',
            'Bind questState_type["AddMiniMapMarkerByScriptName"] = &LuaQuestState::AddMiniMapMarkerByScriptName;']}
    inputs = [*sources, *(runtime/'FableScriptExtender').glob('*.h'),
        source/'runtime_checks/retail_resources_smoke.cpp', output/'maze_marker_adapter.inc', output/'maze_marker_harness.cpp', output/'maze_lifecycle_harness.cpp']
    digest = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    report['inputs'] = {str(p): digest(p) for p in inputs}
    def run(name, command):
        result = subprocess.run(command, cwd=output, env=env, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout)
        report['commands'].append({'name': name, 'exitCode': result.returncode})
        if result.returncode: raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,sources)])
    objects = [str(output/(p.stem+'.obj')) for p in sources]
    run('marker-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(vendor),'/I'+str(lua),
        str(output/'maze_marker_harness.cpp'),*objects,'/Fe:maze-marker.exe'])
    report['compiledTest'] = run('marker-test',[str(output/'maze-marker.exe')])
    run('lifecycle-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(vendor),'/I'+str(lua),
        str(output/'maze_lifecycle_harness.cpp'),*objects,'/Fe:maze-lifecycle.exe'])
    report['compiledLifecycleTest'] = run('lifecycle-test',[str(output/'maze-lifecycle.exe')])
    report['executableSha256'] = digest(output/'maze-marker.exe')
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__ == '__main__':
    print(prepare()['compiledTest'])
