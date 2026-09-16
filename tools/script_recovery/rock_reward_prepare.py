"""Stage/compile the dynamic Rock reward adapter against actual FSE types."""
import hashlib
import json
import shutil
import subprocess
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.rock_rewards import recover
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    phase,evidence=recover()
    source=Path(__file__).parent;output=ROOT/'work/rock_trigger_converter/reward_proposal'
    output.mkdir(parents=True,exist_ok=True)
    names=('rock_reward_adapter.inc','rock_reward_harness.cpp')
    for name in names:shutil.copyfile(source/name,output/name)
    (output/'rock_rewards.lua').write_text(phase)
    env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
    lua=runtime/'Vendor/lua';c_sources=sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c','luac.c','onelua.c'))
    report={'status':'pending integration; no runtime checkout changes','runtimeChanged':False,
        'nativeEvidence':evidence,'commands':[],
        'integration':['Declare/bind LuaQuestState::AddRockTrollReward(CScriptThing*,int) using staged adapter; caller keeps self resource alive.', 'Native dynamic CDefString conversion is used, including -1 empty output. Candidate stays disabled; no runtime files changed.']}
    inputs=[*c_sources,*(runtime/'FableScriptExtender').glob('*.h'),source/'runtime_checks/retail_resources_smoke.cpp',
        output/'rock_rewards.lua',*[output/name for name in names]]
    report['inputs']={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    def run(name,command):
        result=subprocess.run(command,cwd=output,env=env,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
        (output/(name+'.log')).write_text(result.stdout);report['commands'].append({'name':name,'exitCode':result.returncode})
        if result.returncode:raise RuntimeError(result.stdout)
        return result.stdout.strip()
    run('lua-build',[compiler,'/nologo','/c','/TC','/MD','/O2','/I'+str(lua),*map(str,c_sources)])
    run('capability-build',[compiler,'/nologo','/EHsc','/std:c++17','/MD','/O2',
        '/I'+str(output),'/I'+str(source),'/I'+str(runtime/'FableScriptExtender'),'/I'+str(runtime/'Vendor'),'/I'+str(lua),
        str(output/'rock_reward_harness.cpp'),*[str(output/(p.stem+'.obj')) for p in c_sources],'/Fe:rock-reward.exe'])
    report['compiledTest']=run('capability-test',[str(output/'rock-reward.exe')])
    report['executableSha256']=hashlib.sha256((output/'rock-reward.exe').read_bytes()).hexdigest()
    executable=(output/'rock-reward.exe').read_bytes()
    pe=int.from_bytes(executable[0x3c:0x40],'little')
    machine=int.from_bytes(executable[pe+4:pe+6],'little')
    if machine!=0x14c:raise ValueError('Expected actual x86 proposal executable')
    report['peMachine']='0x014c (x86)'
    (output/'INTEGRATION.md').write_text("""# Reward adapter (compiled, pending integration)

Reproduce: `python -m tools.script_recovery.rock_reward_prepare`.
Add/bind the staged method to LuaQuestState. It reloads the definition pointer
for each reward, converts native CDefString +730/+734 through415D70, calls
AddItemToContainer on the original actor, then destroys the CString even empty.
Existing callers and runtime checkout remain unchanged.

The x86 harness compiles the actual adapter unchanged against FSE types and real
Lua. A private test-process address arena routes its ASLR conversion call to an
engine double; no game or runtime process is touched. Tests exercise native-address
selection, definition replacement between calls, -1/empty output, conversion and
consumer exceptions, outer actual LuaRetailResources cleanup, continuation
lifetime and invalid inputs. Original conversion instructions are independently
covered by test_rock_rewards. This is not gameplay or engine fault certification.

Registration, remaining actor phases, scheduler and manager teardown/save-restore
remain pending. The staged candidate continues to register no quest.
""")
    (output/'proposal.json').write_text(json.dumps(report,indent=2)+'\n')
    return report


if __name__=='__main__':print(prepare()['compiledTest'])
