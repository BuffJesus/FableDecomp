"""Stage/compile Rock trigger capabilities against the actual resource scope."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.rock_trigger_recovery import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    _,evidence=recover()
    source=Path(__file__).parent;output=ROOT/'work/rock_trigger_converter/runtime_proposal'
    output.mkdir(parents=True,exist_ok=True)
    names=('rock_trigger_proximity.inc','rock_trigger_spawn_method.inc','rock_trigger_capability_harness.cpp')
    for name in names:shutil.copyfile(source/name,output/name)
    original=(runtime/'FableScriptExtender/LuaRetailResources.h').read_text()
    header=original
    for old,new in [('    void DestroyThing(unsigned id)',
                    '    #include "rock_trigger_spawn_method.inc"\n    void DestroyThing(unsigned id)'),
                   ('    type["DestroyThing"] = &LuaRetailResources::DestroyThing;',
                    '    type["CreateCreatureAtThingPosition"] = &LuaRetailResources::CreateCreatureAtThingPosition;\n    type["DestroyThing"] = &LuaRetailResources::DestroyThing;')]:
        if header.count(old)!=1:raise ValueError('Current retail resource source correspondence changed')
        header=header.replace(old,new)
    (output/'LuaRetailResources.h').write_text(header)
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua=runtime/'Vendor/lua';c_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'pending integration; no runtime checkout changes','runtimeChanged':False,
        'nativeEvidence':evidence,'commands':[],
        'originalResourceHeaderSha256':hashlib.sha256(original.encode()).hexdigest(),
        'integration':[
            'Add/bind LuaQuestState::GetRockTrollTriggerProximity using rock_trigger_proximity.inc; reads actual definition pointer and float on every call.',
            'Add rock_trigger_spawn_method.inc to LuaRetailResources public methods and register CreateCreatureAtThingPosition; staged header contains the exact changes.',
            'Use existing inactive draft/M_RTFERockTrollTrigger.lua, which already declares these capabilities; do not activate or overwrite canonical output.',
            'Entity pre-entry cancellation timing and bound-condition teardown remain integration requirements. This capability does not alter scheduler policy.']}
    inputs=[*c_sources,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',
        output/'LuaRetailResources.h',*[output/name for name in names]]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout);report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    run('capability-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'rock_trigger_capability_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in c_sources],'/Fe:rock-trigger-capability.exe'])
    report['compiledTest']=run('capability-test',[str(output/'rock-trigger-capability.exe')])
    report['executableSha256']=hashlib.sha256((output/'rock-trigger-capability.exe').read_bytes()).hexdigest()
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(prepare()['compiledTest'])
