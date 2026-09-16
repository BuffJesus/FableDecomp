"""Stage and compile the isolated Scythe adapter; never edit the runtime checkout."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment, find_vcvars
from tools.script_recovery.scythe_cutscene_audit import audit


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    checked = audit()
    if not checked['status'].startswith('checked'):
        raise ValueError(checked)
    output = ROOT / 'work/scythe_converter/runtime_proposal'
    output.mkdir(parents=True, exist_ok=True)
    source = Path(__file__).parent
    control_host = (runtime / 'FableScriptExtender/LuaEntityAPI.cpp').read_text()
    checked_methods = '\n'.join(control_host[control_host.index(start):control_host.index(end)]
        for start, end in [('void LuaEntityAPI::SelectControlHandle', 'LuaEntityAPI::~LuaEntityAPI'),
                           ('bool LuaEntityAPI::AcquireControl', 'void LuaEntityAPI::MakeBehavioral')])
    if checked_methods != (source / 'scythe_control_host_snapshot.inc').read_text():
        raise ValueError('Current runtime acquisition differs from the checked host snapshot')
    if (runtime/'FableScriptExtender/LuaRetailCondition.h').read_text() != (source/'scythe_condition_host_snapshot.h').read_text():
        raise ValueError('Current runtime condition differs from checked host snapshot')
    for name in ('scythe_oracle_adapter.inc', 'scythe_oracle_adapter_harness.cpp',
                 'scythe_control_harness.cpp', 'scythe_control_host_snapshot.inc'):
        shutil.copyfile(source / name, output / name)
    shutil.copyfile(source/'scythe_condition_harness.cpp', output/'scythe_condition_harness.cpp')
    shutil.copyfile(source/'scythe_alive_registration.inc', output/'alive-registration.inc')
    for name in ('scythe_thing_adapters.inc','scythe_thing_harness.cpp'):
        shutil.copyfile(source/name,output/name)
    report = {'status': 'pending integration', 'runtimeChanged': False,
              'nativeAudit': checked, 'commands': []}
    env = compiler_environment(find_vcvars())
    compiler = shutil.which('cl.exe', path=env['PATH'])
    vendor = runtime / 'Vendor'
    lua = vendor / 'lua'
    sources = sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c', 'luac.c', 'onelua.c'))
    inputs = [*sources, *(runtime / 'FableScriptExtender').glob('*.h'),
              runtime / 'FableScriptExtender/LuaQuestState.cpp',
              runtime / 'FableScriptExtender/LuaManager.cpp',
              runtime / 'FableScriptExtender/LuaEntityAPI.cpp',
              source / 'runtime_checks/retail_resources_smoke.cpp',
              output / 'scythe_oracle_adapter.inc', output / 'scythe_oracle_adapter_harness.cpp',
              output / 'scythe_control_harness.cpp', output / 'scythe_control_host_snapshot.inc']
    inputs += [output/name for name in ('scythe_condition_harness.cpp','alive-registration.inc',
                                        'scythe_thing_adapters.inc','scythe_thing_harness.cpp')]
    digest = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    report['inputs'] = {str(path): digest(path) for path in inputs}
    def run(name, command):
        result = subprocess.run(command, cwd=output, env=env, text=True,
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        (output / (name + '.log')).write_text(result.stdout)
        report['commands'].append({'name': name, 'exitCode': result.returncode})
        if result.returncode:
            raise RuntimeError(result.stdout)
        return result.stdout
    run('lua-build', [compiler, '/nologo', '/c', '/TC', '/MD', '/O2', '/I' + str(lua), *map(str, sources)])
    objects = [str(output / (p.stem + '.obj')) for p in sources]
    run('adapter-build', [compiler, '/nologo', '/EHsc', '/std:c++17', '/MD', '/O2',
        '/I' + str(source), '/I' + str(runtime / 'FableScriptExtender'),
        '/I' + str(vendor), '/I' + str(lua), str(output / 'scythe_oracle_adapter_harness.cpp'),
        *objects, '/Fe:scythe-oracle-adapter.exe'])
    report['compiledTest'] = run('adapter-test', [str(output / 'scythe-oracle-adapter.exe')]).strip()
    run('control-build', [compiler, '/nologo', '/EHsc', '/std:c++17', '/MD', '/O2',
        '/I' + str(source), '/I' + str(runtime / 'FableScriptExtender'),
        '/I' + str(vendor), '/I' + str(lua), str(output / 'scythe_control_harness.cpp'),
        *objects, '/Fe:scythe-control.exe'])
    report['compiledControlTest'] = run('control-test', [str(output / 'scythe-control.exe')]).strip()
    run('condition-build', [compiler, '/nologo', '/EHsc', '/std:c++17', '/MD', '/O2',
        '/I' + str(output), '/I' + str(source), '/I' + str(runtime / 'FableScriptExtender'),
        '/I' + str(vendor), '/I' + str(lua), str(output/'scythe_condition_harness.cpp'),
        *objects, '/Fe:scythe-condition.exe'])
    report['compiledConditionTest'] = run('condition-test', [str(output/'scythe-condition.exe')]).strip()
    run('thing-build', [compiler, '/nologo', '/EHsc', '/std:c++17', '/MD', '/O2',
        '/I' + str(output), '/I' + str(source), '/I' + str(runtime/'FableScriptExtender'),
        '/I' + str(vendor), '/I' + str(lua), str(output/'scythe_thing_harness.cpp'),
        *objects, '/Fe:scythe-thing.exe'])
    report['compiledThingTest'] = run('thing-test', [str(output/'scythe-thing.exe')]).strip()
    if any(digest(Path(path)) != expected for path, expected in report['inputs'].items()):
        raise ValueError('An input changed during the compiled checks')
    report['candidateSha256'] = hashlib.sha256((output / 'scythe_oracle_adapter.inc').read_bytes()).hexdigest()
    from tools.script_recovery.scythe_runtime_candidate import build
    report['luaCandidate'] = build()['runtimeProposal']
    (output / 'proposal.json').write_text(json.dumps(report, indent=2) + '\n')
    (output / 'INTEGRATION.md').write_text('''# Pending Scythe adapter integration

Add `void RunScytheOracleCutscene(CScriptThing*, sol::this_state);` to LuaQuestState.
Include `scythe_oracle_adapter.inc` after the host and LuaEntityAPI definitions.
Also add `void FaceThingByScriptName(CScriptThing*, const std::string&, bool);`
and `void SpawnScytheAndRemoveMarker(CScriptThing*);`, and include
`scythe_thing_adapters.inc` in the same implementation unit.
In LuaManager.cpp add:
`questState_type["RunScytheOracleCutscene"] = &LuaQuestState::RunScytheOracleCutscene;`
`questState_type["FaceThingByScriptName"] = &LuaQuestState::FaceThingByScriptName;`
`questState_type["SpawnScytheAndRemoveMarker"] = &LuaQuestState::SpawnScytheAndRemoveMarker;`
Existing RunCutsceneWithSetup and its callers remain unchanged.

FaceThingByScriptName is general: actor/name/snap are explicit. A key CString
survives lookup and facing; the actual returned Thing (including empty output) is
destroyed before the key. SpawnScytheAndRemoveMarker releases definition/script
strings before marker removal and destroys the spawned Thing output afterward.
The candidate uses the existing RegisterBoundAliveCondition binding before its
first frame. Its ABI, clone ownership and local destruction are checked.

The caller must already own a movie and Scythe control. The adapter borrows both,
acquires a fresh Hero resource exactly once at priority 4, and copies the actual
output even when acquisition returns false (failure can still populate output).
It passes a constructed empty input map, null flags, setup=false, skippable=true,
and brackets camera true/false. It does not insert a termination check or yield.
Input and actor maps and the local Hero resource are released after the macro;
the caller retains its movie, pause, and Scythe control.

Compiled tests use real x86 API types and sol/Lua with engine entry-point doubles.
They cover successful acquisition, empty and populated failed output, macro
exceptions, retained caller ownership, and a missing caller movie. This is not
an in-game validation. The candidate remains disabled pending integration and
the separately documented condition, scheduler, and borrowed-resource equivalence review.
''')
    return report


if __name__ == '__main__':
    result = prepare()
    print(json.dumps({k: result[k] for k in ('status', 'runtimeChanged', 'compiledTest')}, indent=2))
