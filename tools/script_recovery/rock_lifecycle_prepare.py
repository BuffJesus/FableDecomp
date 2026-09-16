"""Stage/compile the final Rock targeting adapter against actual FSE types."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.rock_lifecycle import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    phase,evidence=recover()
    source=Path(__file__).parent;output=ROOT/'work/rock_trigger_converter/lifecycle_proposal'
    output.mkdir(parents=True,exist_ok=True)
    names=('rock_lifecycle_adapter.inc','rock_lifecycle_harness.cpp')
    for name in names:shutil.copyfile(source/name,output/name)
    (output/'rock_lifecycle.lua').write_text(phase)
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua=runtime/'Vendor/lua';c_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'pending integration; no runtime checkout changes','runtimeChanged':False,
        'nativeEvidence':evidence,'commands':[],
        'integration':['Declare and bind InitializeRockTrollMarker(CScriptThing*) from the staged inc; preserve existing entrypoints.', 'Direct persistent call even for empty Data, then scoped CString marker consumer. No runtime edits.']}
    inputs=[*c_sources,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',
        output/'rock_lifecycle.lua',*[output/name for name in names]]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout);report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    run('capability-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'rock_lifecycle_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in c_sources],'/Fe:rock-lifecycle.exe'])
    report['compiledTest']=run('capability-test',[str(output/'rock-lifecycle.exe')])
    report['executableSha256']=hashlib.sha256((output/'rock-lifecycle.exe').read_bytes()).hexdigest()
    executable=(output/'rock-lifecycle.exe').read_bytes()
    pe=int.from_bytes(executable[0x3c:0x40],'little')
    machine=int.from_bytes(executable[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Expected actual x86 proposal executable')
    report['peMachine']='0x014c (x86)'
    (output/'INTEGRATION.md').write_text("""# Marker initialization: compiled, unapplied

Reproduce: python -m tools.script_recovery.rock_lifecycle_prepare.
Declare and bind the separate InitializeRockTrollMarker(CScriptThing*) method.
The actual FSE types and real Lua harness exercise populated/empty bound Things,
no IsNull filtering, persistent-before-CString construction, marker-before-string
destruction, and errors unwinding the enclosing resource callback. API doubles
stand in for the engine. Existing persistence/marker wrappers are unchanged.
RAII error handling is a host policy, not a proved native exception policy.
All scheduler, teardown, and unresolved TrollAwake initialization gates remain.
""")
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(prepare()['compiledTest'])
