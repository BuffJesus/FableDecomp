"""Stage/compile the final Rock targeting adapter against actual FSE types."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    phase,evidence=recover()
    source=Path(__file__).parent;output=ROOT/'work/bully_converter/talk_proposal'
    output.mkdir(parents=True,exist_ok=True)
    names=('bully_talk_predicate.inc','bully_talk_harness.cpp')
    for name in names:shutil.copyfile(source/name,output/name)
    (output/'bully_talk.lua').write_text(phase)
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua=runtime/'Vendor/lua';c_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'pending integration; no runtime checkout changes','runtimeChanged':False,
        'nativeEvidence':evidence,'commands':[],
        'integration':['Add staged BullyTalkedWithTeddy method to LuaRetailResources and bind it; preserve existing wrappers.', 'Raw Hero output is consumed including null; both native CString scopes overlap until predicate result.']}
    inputs=[*c_sources,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',
        output/'bully_talk.lua',*[output/name for name in names]]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout);report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    run('capability-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'bully_talk_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in c_sources],'/Fe:bully-talk.exe'])
    report['compiledTest']=run('capability-test',[str(output/'bully-talk.exe')])
    report['executableSha256']=hashlib.sha256((output/'bully-talk.exe').read_bytes()).hexdigest()
    executable=(output/'bully-talk.exe').read_bytes()
    pe=int.from_bytes(executable[0x3c:0x40],'little')
    machine=int.from_bytes(executable[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Expected actual x86 proposal executable')
    report['peMachine']='0x014c (x86)'
    (output/'INTEGRATION.md').write_text("""# Bully talk/teddy predicate ? compiled proposal, unapplied

Reproduce: python -m tools.script_recovery.bully_talk_prepare.
Add/bind the staged method on LuaRetailResources. Existing wrappers stay unchanged.
Native DBB613..DBB6B9: Hero CString -> talk query -> optional teddy CString ->
fresh raw Hero -> possession -> teddy destructor -> Hero CString destructor.
The raw Hero pointer is passed even when null. No extra native Thing copy.
Actual FSE types/real Lua exercise24truth/null/error policies; API doubles stand
in for the engine. Primary errors unwind both CString and enclosing resource
scopes. This host exception policy is not native exception-flow proof.
""")
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(prepare()['compiledTest'])
