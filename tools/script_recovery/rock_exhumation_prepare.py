"""Compile the exhumation continuation against actual FSE resource types."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.rock_exhumation import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    phase,evidence=recover()
    source=Path(__file__).parent;output=ROOT/'work/rock_trigger_converter/exhumation_proposal'
    output.mkdir(parents=True,exist_ok=True)
    names=('rock_exhumation_harness.cpp',)
    for name in names:shutil.copyfile(source/name,output/name)
    from tools.script_recovery.rock_actor_phases import recover as recover_phases
    prefix,_=recover_phases()
    (output/'rock_exhumation.lua').write_text(prefix+'\n'+phase)
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua=runtime/'Vendor/lua';c_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'pending integration; no runtime checkout changes','runtimeChanged':False,
        'nativeEvidence':evidence,'commands':[],
        'integration':['Uses existing actual LuaRetailResources map/movie/macro/pause APIs without modifying runtime.', 'CreateRetainedThingThread remains pending and is an explicit engine double in this gate. Actor Main and scheduler/teardown remain incomplete.']}
    inputs=[*c_sources,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',
        output/'rock_exhumation.lua',*[output/name for name in names]]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout);report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    run('capability-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'rock_exhumation_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in c_sources],'/Fe:rock-exhumation.exe'])
    report['compiledTest']=run('capability-test',[str(output/'rock-exhumation.exe')])
    report['executableSha256']=hashlib.sha256((output/'rock-exhumation.exe').read_bytes()).hexdigest()
    executable=(output/'rock-exhumation.exe').read_bytes()
    pe=int.from_bytes(executable[0x3c:0x40],'little')
    machine=int.from_bytes(executable[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Expected actual x86 proposal executable')
    report['peMachine']='0x014c (x86)'
    (output/'INTEGRATION.md').write_text("""# Exhumation resource gate (unapplied)

Reproduce: python -m tools.script_recovery.rock_exhumation_prepare.
No new movie/map adapter is needed: the phase uses the existing LuaRetailResources
NewActorMap/SetActor/StartMovie/Pause/RunMacro/DestroyMovie/DestroyActorMap APIs.
Native macro operands match RunMacro(false,true) with null flag/input maps.

The actual-FSE x86/Lua gate covers map resource reference copies including empty
outputs, borrowed movie scope, macro flags, cleanup order, macro/thread errors,
and cancellation set during the macro without an invented post-macro guard.
The thread registration method is an engine double and is still a pending host
capability. This gate does not certify native scheduler or gameplay behavior.
The entire candidate remains disabled; no runtime or canonical files are changed.
""")
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(prepare()['compiledTest'])
