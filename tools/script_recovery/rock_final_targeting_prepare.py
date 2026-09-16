"""Stage/compile the final Rock targeting adapter against actual FSE types."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.rock_final_targeting import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    phase,evidence=recover()
    source=Path(__file__).parent;output=ROOT/'work/rock_trigger_converter/final_targeting_proposal'
    output.mkdir(parents=True,exist_ok=True)
    names=('rock_final_targeting_adapter.inc','rock_final_targeting_harness.cpp')
    for name in names:shutil.copyfile(source/name,output/name)
    (output/'rock_final_targeting.lua').write_text(phase)
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua=runtime/'Vendor/lua';c_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'pending integration; no runtime checkout changes','runtimeChanged':False,
        'nativeEvidence':evidence,'commands':[],
        'integration':['Declare/bind staged TargetRockTrollAtHero(CScriptThing*) method; existing generic targeting wrappers unchanged.', 'Two raw GetHero results are passed directly including null. Candidate remains disabled and runtime unchanged.']}
    inputs=[*c_sources,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',
        output/'rock_final_targeting.lua',*[output/name for name in names]]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout);report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    run('capability-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'rock_final_targeting_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in c_sources],'/Fe:rock-final-targeting.exe'])
    report['compiledTest']=run('capability-test',[str(output/'rock-final-targeting.exe')])
    report['executableSha256']=hashlib.sha256((output/'rock-final-targeting.exe').read_bytes()).hexdigest()
    executable=(output/'rock-final-targeting.exe').read_bytes()
    pe=int.from_bytes(executable[0x3c:0x40],'little')
    machine=int.from_bytes(executable[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Expected actual x86 proposal executable')
    report['peMachine']='0x014c (x86)'
    (output/'INTEGRATION.md').write_text("""# Final targeting adapter ? compiled, unapplied

Reproduce: python -m tools.script_recovery.rock_final_targeting_prepare.
Bind the staged quest method after review. It takes the bound raw Thing and
passes two fresh native GetHero outputs directly to ForceLook/GiveBestEnemy,
including null outputs. It adds no native Thing copy/refcount ownership.

The actual-FSE x86/Lua gate covers all null/non-null combinations, separate Hero
outputs, unchanged native Info counts and consumer errors unwinding an enclosing
actual resource scope. Game APIs are engine doubles; runtime files are unchanged.
Generic shared-pointer/null-filtering targeting wrappers keep existing behavior.
All other actor/host lifecycle, scheduler and teardown integration gates remain.
""")
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(prepare()['compiledTest'])
