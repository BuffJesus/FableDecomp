"""Stage/compile the final Rock targeting adapter against actual FSE types."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.rock_actor_phases import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    phase,evidence=recover()
    source=Path(__file__).parent;output=ROOT/'work/rock_trigger_converter/copied_thing_proposal'
    output.mkdir(parents=True,exist_ok=True)
    names=('rock_copied_thing.h','rock_copied_thing_harness.cpp')
    for name in names:shutil.copyfile(source/name,output/name)
    (output/'rock_copied_thing.lua').write_text(phase)
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua=runtime/'Vendor/lua';c_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'pending integration; no runtime checkout changes','runtimeChanged':False,
        'nativeEvidence':evidence,'commands':[],
        'integration':['Ownership core only; do not bind as completed WithCopiedThreadThing until condition/message/healthbar consumers are implemented.', 'Retained scheduler argument ownership, dispatch and quiescence are separate unresolved gates.']}
    inputs=[*c_sources,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',
        output/'rock_copied_thing.lua',*[output/name for name in names]]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout);report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    run('capability-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'rock_copied_thing_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in c_sources],'/Fe:rock-copied-thing.exe'])
    report['compiledTest']=run('capability-test',[str(output/'rock-copied-thing.exe')])
    report['executableSha256']=hashlib.sha256((output/'rock-copied-thing.exe').read_bytes()).hexdigest()
    executable=(output/'rock-copied-thing.exe').read_bytes()
    pe=int.from_bytes(executable[0x3c:0x40],'little')
    machine=int.from_bytes(executable[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Expected actual x86 proposal executable')
    report['peMachine']='0x014c (x86)'
    (output/'INTEGRATION.md').write_text("""# Copied Thing ownership core: compiled, unapplied

Reproduce: python -m tools.script_recovery.rock_copied_thing_prepare.
EC52F0 copies the three-word CScriptThing and increments Info even for empty
Data. This scope models only the per-invocation argument; it does not own or
replace the scheduler's distinct retained argument. No raw Thing or shared native
ownership is exported to Lua. Atomic GetHealth uses the live local address.
Return/cancellation/error closes the argument once; escaped userdata is inert.
Cleanup exceptions preserve a primary callback error. This is explicit host
error policy, not a recovered native exception guarantee.

Actual FSE types/real Lua test24 policies across empty Data, absent Info,
return/cancellation/body error and cleanup error. Engine API doubles validate
copy address, full word identity and refcounts. Existing native dispatcher tests
independently execute EC52F0 and EC5330; stored argument survives callback scope.

This is NOT the complete WithCopiedThreadThing capability: parent alive condition,
message consumers and atomic healthbar state writes remain to implement. Native
retained thread scheduling, yield/resume integration and no-future-dispatch teardown
are unproved. Do not wire or enable the gameplay candidate using this core alone.
""")
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(prepare()['compiledTest'])
