"""Stage/compile the persistent region-message scope against actual FSE types."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.rock_region_message_native import verify
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    evidence=verify()
    source=Path(__file__).parent;output=ROOT/'work/rock_trigger_converter/region_message_proposal'
    output.mkdir(parents=True,exist_ok=True)
    names=('rock_region_message.h','rock_region_message_adapter.inc','rock_region_message_harness.cpp')
    for name in names:shutil.copyfile(source/name,output/name)
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua=runtime/'Vendor/lua';c_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'pending integration; no runtime checkout changes','runtimeChanged':False,
        'nativeEvidence':evidence,'commands':[],
        'integration':['Add rock_region_message.h and adapter method to LuaQuestState; register userdata and quest method as documented in adapter. Existing MsgOnRegionLoaded stays unchanged.', 'Root candidate already requires this capability; scheduler, teardown, and whole-port state wiring remain pending.']}
    inputs=[*c_sources,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',
        *[output/name for name in names]]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout);report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    run('capability-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'rock_region_message_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in c_sources],'/Fe:rock-region-message.exe'])
    report['compiledTest']=run('capability-test',[str(output/'rock-region-message.exe')])
    report['executableSha256']=hashlib.sha256((output/'rock-region-message.exe').read_bytes()).hexdigest()
    executable=(output/'rock-region-message.exe').read_bytes()
    pe=int.from_bytes(executable[0x3c:0x40],'little')
    machine=int.from_bytes(executable[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Expected actual x86 proposal executable')
    report['peMachine']='0x014c (x86)'
    (output/'INTEGRATION.md').write_text('''# Scoped region-message proposal (pending integration)

Reproduce with `python -m tools.script_recovery.rock_region_message_prepare`.
The root candidate already calls `quest:WithRegionLoadedMessage(callback)`.
Add the supplied header and method declaration/body to LuaQuestState, register
`RegisterRockRegionMessage(lua)`, then bind the method as shown in the adapter.
The existing string-or-nil polling API keeps its current behavior.

One native default-constructed CString survives every poll and the callback's
success side effects. Poll returns the native bool without inspecting output.
Cancellation returns from the callback and destroys that CString once. Escaped
userdata becomes inert after closure. Callback errors preserve the primary
error while attempting cleanup; simulated C++ cleanup errors do not establish
recovery from engine faults. The harness compiles the actual adapter with FSE
CString/API/resource types and real Lua; it uses engine doubles, not the game.

Scheduler entry timing, scheduler quiescence, manager teardown, whole-port
integer health-bar state wiring, and remaining actor/helper recovery still
require integration. The root candidate remains disabled; this is not evidence
of gameplay completeness. Native addresses and constructor/destructor/loop
byte guards are recorded in proposal.json.
''')
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(prepare()['compiledTest'])
